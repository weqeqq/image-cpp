
#pragma once

#include <algorithm>
#include <image/core/buffer.h>

namespace Image {
//
enum class Blending {
  Normal
};
class BlendFn {
public:
  void operator()(Buffer<> &bg, const Buffer<> &fg, unsigned xoffset, unsigned yoffset, Blending blending) const {
    for (auto bgrow = yoffset;
              bgrow < std::min(yoffset + fg.RowCount(), bg.RowCount());
              bgrow++) {
      for (auto bgcol = xoffset;
                bgcol < std::min(xoffset + fg.ColumnCount(), bg.ColumnCount());
                bgcol++) {
        auto bgpix = bg(bgrow, bgcol);
        auto fgpix = fg(bgrow - yoffset, bgcol - xoffset);
        U8 blended[4];
        std::copy(bgpix, bgpix + 4, blended);
        Blend(blended, fgpix, blending); Blenda(bgpix, blended);
      }
    }
  }
private:
  void Blenda(U8 *bg, const U8 *fg) const {
    float alpha = float(fg[3]) / 0xff;
    for (auto ch = 0u; ch < 3; ch++) {
      bg[ch] = (bg[ch] * (1.0f - alpha)) + (fg[ch] * alpha);
    }
    bg[3] = std::max(bg[3], fg[3]);
  }
  void BlendNormal(U8 *bg, const U8 *fg) const {
    for (auto ch = 0u; ch < 4; ch++) {
      bg[ch] = fg[ch];
    }
  }
  void Blend(U8 *bg, const U8 *fg, Blending blending) const {
    switch (blending) {
      case Blending::Normal: return BlendNormal(bg, fg);
    }
  }
}; // class BlendFn
inline constexpr BlendFn Blend = BlendFn();
} // namespace Image
