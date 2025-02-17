//
// Copyright RIME Developers
// Distributed under the BSD License
//
// 2011-09-11 GONG Chen <chen.sst@gmail.com>
//
#include <rime/common.h>
#include <rime/composition.h>
#include <rime/context.h>
#include <rime/engine.h>
#include <rime/key_event.h>
#include <rime/key_table.h>
#include <rime/menu.h>
#include <rime/schema.h>
#include <rime/gear/selector.h>

namespace rime {

// Direction of candidate list.
using Direction = int;
constexpr Direction kDirectionVoid = -1;
constexpr Direction kDirectionDown = 0;
constexpr Direction kDirectionLeft = 1;
constexpr Direction kDirectionUp = 2;
constexpr Direction kDirectionRight = 3;

Selector::Selector(const Ticket& ticket) : Processor(ticket) {
}

ProcessResult Selector::ProcessKeyEvent(const KeyEvent& key_event) {
  if (key_event.release() || key_event.alt())
    return kNoop;
  Context* ctx = engine_->context();
  if (ctx->composition().empty())
    return kNoop;
  Segment& current_segment(ctx->composition().back());
  if (!current_segment.menu || current_segment.HasTag("raw"))
    return kNoop;
  bool is_linear_candidate_list = ctx->get_option("_linear");
  bool is_vertical_text = ctx->get_option("_vertical");
  // Deprecated. equivalent to {_linear: true, _vertical: false}
  bool is_horizontal_layout = ctx->get_option("_horizontal");
  Direction next_candidate = kDirectionDown;
  Direction next_page = kDirectionDown;
  if (is_vertical_text) {
    // +90 degrees
    next_candidate = (next_candidate + 1) % 4;
    next_page = (next_page + 1) % 4;
  }
  if (is_linear_candidate_list || is_horizontal_layout) {
    // -90 degrees
    next_candidate = (next_candidate + 3) % 4;
  }
  if (next_page == next_candidate) {
    // this is no-op. just to clarify that arrow keys don't change page.
    next_page = kDirectionVoid;
  }
  int ch = key_event.keycode();
  if (ch == XK_Prior || ch == XK_KP_Prior) {
    PageUp(ctx);
    return kAccepted;
  }
  if (ch == XK_Next || ch == XK_KP_Next) {
    PageDown(ctx);
    return kAccepted;
  }
  if (ch == XK_Up || ch == XK_KP_Up) {
    if (next_candidate == kDirectionDown) {
      CursorUp(ctx);
    } else if (next_page == kDirectionDown) {
      PageUp(ctx);
    }
    return kAccepted;
  }
  if (ch == XK_Down || ch == XK_KP_Down) {
    if (next_candidate == kDirectionDown) {
      CursorDown(ctx);
    } else if (next_page == kDirectionDown) {
      PageDown(ctx);
    }
    return kAccepted;
  }
  if (ch == XK_Left || ch == XK_KP_Left) {
    if (!key_event.ctrl() &&
        !key_event.shift() &&
        ctx->caret_pos() == ctx->input().length()) {
      if (next_candidate == kDirectionRight &&
          CursorUp(ctx)) {
        return kAccepted;
      }
      if (next_candidate == kDirectionLeft) {
        CursorDown(ctx);
        return kAccepted;
      }
      if (next_page == kDirectionLeft) {
        PageDown(ctx);
        return kAccepted;
      }
    }
    return kNoop;
  }
  if (ch == XK_Right || ch == XK_KP_Right) {
    if (!key_event.ctrl() &&
        !key_event.shift() &&
        ctx->caret_pos() == ctx->input().length()) {
      if (next_candidate == kDirectionRight) {
        CursorDown(ctx);
        return kAccepted;
      }
      if (next_candidate == kDirectionLeft) {
        CursorUp(ctx);
        return kAccepted;
      }
      if (next_page == kDirectionLeft) {
        PageUp(ctx);
        return kAccepted;
      }
    }
    return kNoop;
  }
  if (ch == XK_Home || ch == XK_KP_Home) {
    return Home(ctx) ? kAccepted : kNoop;
  }
  if (ch == XK_End || ch == XK_KP_End) {
    return End(ctx) ? kAccepted : kNoop;
  }
  int index = -1;
  const string& select_keys(engine_->schema()->select_keys());
  if (!select_keys.empty() &&
      !key_event.ctrl() &&
      ch >= 0x20 && ch < 0x7f) {
    size_t pos = select_keys.find((char)tolower(ch));
    if (pos != string::npos) {
      index = static_cast<int>(pos);
    }
  }
  else if (ch >= XK_0 && ch <= XK_9)
    index = ((ch - XK_0) + 9) % 10;
  else if (ch >= XK_KP_0 && ch <= XK_KP_9)
    index = ((ch - XK_KP_0) + 9) % 10;
  if (index >= 0) {
    SelectCandidateAt(ctx, index);
    return kAccepted;
  }
  // not handled
  return kNoop;
}

bool Selector::PageUp(Context* ctx) {
  Composition& comp = ctx->composition();
  if (comp.empty())
    return false;
  int page_size = engine_->schema()->page_size();
  int selected_index = comp.back().selected_index;
  int index = selected_index < page_size ? 0 : selected_index - page_size;
  comp.back().selected_index = index;
  comp.back().tags.insert("paging");
  return true;
}

bool Selector::PageDown(Context* ctx) {
  Composition& comp = ctx->composition();
  if (comp.empty() || !comp.back().menu)
    return false;
  int page_size = engine_->schema()->page_size();
  int index = comp.back().selected_index + page_size;
  int page_start = (index / page_size) * page_size;
  int candidate_count = comp.back().menu->Prepare(page_start + page_size);
  if (candidate_count <= page_start) {
    bool page_down_cycle = engine_->schema()->page_down_cycle();
    if (page_down_cycle) {// Cycle back to page 1 if true
      index = 0;
    } else {
      return false;
    }
  } else if (index >= candidate_count) {
    index = candidate_count - 1;
  }
  comp.back().selected_index = index;
  comp.back().tags.insert("paging");
  return true;

}

bool Selector::CursorUp(Context* ctx) {
  Composition& comp = ctx->composition();
  if (comp.empty())
    return false;
  int index = comp.back().selected_index;
  if (index <= 0)
    return false;
  comp.back().selected_index = index - 1;
  comp.back().tags.insert("paging");
  return true;
}

bool Selector::CursorDown(Context* ctx) {
  Composition& comp = ctx->composition();
  if (comp.empty() || !comp.back().menu)
    return false;
  int index = comp.back().selected_index + 1;
  int candidate_count = comp.back().menu->Prepare(index + 1);
  if (candidate_count <= index)
    return false;
  comp.back().selected_index = index;
  comp.back().tags.insert("paging");
  return true;
}

bool Selector::Home(Context* ctx) {
  if (ctx->composition().empty())
    return false;
  Segment& seg(ctx->composition().back());
  if (seg.selected_index > 0) {
    seg.selected_index = 0;
    return true;
  }
  return false;
}

bool Selector::End(Context* ctx) {
  if (ctx->caret_pos() < ctx->input().length()) {
    // navigator should handle this
    return false;
  }
  // this is cool:
  return Home(ctx);
}


bool Selector::SelectCandidateAt(Context* ctx, int index) {
  Composition& comp = ctx->composition();
  if (comp.empty())
    return false;
  int page_size = engine_->schema()->page_size();
  if (index >= page_size)
    return false;
  int selected_index = comp.back().selected_index;
  int page_start = (selected_index / page_size) * page_size;
  return ctx->Select(page_start + index);
}

}  // namespace rime
