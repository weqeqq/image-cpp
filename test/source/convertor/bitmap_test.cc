

#include <image/convertor/bitmap.h>
#include <gtest/gtest.h>

TEST(BitmapConvertorTest, ConvertFromBitmap) {
  std::vector<std::uint8_t> input = {
    0b11001100, 0b00110011, 0b11000000,
  };
  auto output = Image::BitmapConvertor(input).FromBitmap(20);

  EXPECT_EQ(output, std::vector<std::uint8_t>({
    0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 
    0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 
    0xff, 0xff, 0x00, 0x00
  }));
}

TEST(BitmapConvertorTest, ConvertToBitmap) {
  std::vector<std::uint8_t> input = {
    0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 
    0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 
    0xff, 0xff, 0x00, 0x00
  };
  auto output = Image::BitmapConvertor(input).ToBitmap();

  EXPECT_EQ(output, std::vector<std::uint8_t>({
    0b11001100, 0b00110011, 0b11000000
  }));
}

