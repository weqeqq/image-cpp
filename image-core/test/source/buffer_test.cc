
#include <gtest/gtest.h>
#include <image/buffer.h>
#include <sstream>

class BufferTest : public ::testing::Test {
protected:
  void SetUp() override {
    buffer = Image::Buffer(2, 2);
  }
  Image::Buffer buffer;
};

TEST_F(BufferTest, ElementConstructionAndAccessors) {
  Image::Buffer::Element elem(0xFF, 0x80, 0x40, 0xFF);

  EXPECT_EQ(elem.Red(), 0xFF);
  EXPECT_EQ(elem.Green(), 0x80);
  EXPECT_EQ(elem.Blue(), 0x40);
  EXPECT_EQ(elem.Alpha(), 0xFF);

  EXPECT_EQ(elem[0], 0xFF);
  EXPECT_EQ(elem[1], 0x80);
  EXPECT_EQ(elem[2], 0x40);
  EXPECT_EQ(elem[3], 0xFF);
}

TEST_F(BufferTest, ElementSubscriptOutOfRange) {
  Image::Buffer::Element elem;
  EXPECT_THROW(elem[Image::Buffer::ChannelCount], Image::BufferError::OutOfRange);
}

TEST_F(BufferTest, BufferConstruction) {
  EXPECT_EQ(buffer.GetRowCount(), 2);
  EXPECT_EQ(buffer.GetColumnCount(), 2);
  EXPECT_EQ(buffer.GetLength(), 4); // 2x2 = 4 elements
  EXPECT_EQ(buffer.GetByteCount(), 4 * Image::Buffer::ChannelCount); // 4 elements * 4 channels
}

TEST_F(BufferTest, BufferSubscriptAndAssignment) {
  Image::Buffer::Element elem(0xFF, 0x80, 0x40, 0xFF);
  buffer[0] = elem;

  EXPECT_EQ(buffer[0][0], 0xFF);
  EXPECT_EQ(buffer[0][1], 0x80);
  EXPECT_EQ(buffer[0][2], 0x40);
  EXPECT_EQ(buffer[0][3], 0xFF);

  buffer(1, 1) = Image::Buffer::Element(0xAA, 0xBB, 0xCC, 0xDD);
  EXPECT_EQ(buffer(1, 1)[0], 0xAA);
  EXPECT_EQ(buffer(1, 1)[1], 0xBB);
  EXPECT_EQ(buffer(1, 1)[2], 0xCC);
  EXPECT_EQ(buffer(1, 1)[3], 0xDD);
}

TEST_F(BufferTest, BufferSubscriptOutOfRange) {
  EXPECT_THROW(buffer[buffer.GetLength()], Image::BufferError::OutOfRange);
  EXPECT_THROW(buffer(buffer.GetRowCount(), 0), Image::BufferError::OutOfRange);
  EXPECT_THROW(buffer(0, buffer.GetColumnCount()), Image::BufferError::OutOfRange);
}

TEST_F(BufferTest, BufferIterator) {
  Image::Buffer::Element elem1(0x10, 0x20, 0x30, 0x40);
  Image::Buffer::Element elem2(0x50, 0x60, 0x70, 0x80);
  Image::Buffer::Element elem3(0x90, 0xA0, 0xB0, 0xC0); 
  Image::Buffer::Element elem4(0xD0, 0xE0, 0xF0, 0xFF);
  buffer[0] = elem1;
  buffer[1] = elem2;
  buffer[2] = elem3;
  buffer[3] = elem4;

  auto it = buffer.begin();
  EXPECT_EQ(*it, elem1);
  ++it;
  EXPECT_EQ(*it, elem2);
  ++it;
  EXPECT_EQ(*it, elem3);
  ++it;
  EXPECT_EQ(*it, elem4);
  ++it;
  EXPECT_EQ(it, buffer.end());
}

TEST_F(BufferTest, BufferIteratorIncrement) {
  auto it = buffer.begin();
  auto it2 = it + 1;
  EXPECT_NE(it, it2);
  EXPECT_EQ(it2, ++it);
}

TEST_F(BufferTest, BufferOutputStream) {
  Image::Buffer::Element elem(0xFF, 0x80, 0x40, 0xFF);
  buffer[0] = elem;
  buffer(1, 1) = elem;

  std::ostringstream oss;
  oss << buffer;

  std::string expected = "[0xFF 0x80 0x40 0xFF] [0x00 0x00 0x00 0x00]\n[0x00 0x00 0x00 0x00] [0xFF 0x80 0x40 0xFF]";
  EXPECT_EQ(oss.str(), expected);
}

TEST_F(BufferTest, SubscriptOperatorReturnEquality) {
  Image::Buffer::Element elem(0xFF, 0x80, 0x40, 0xFF);
  buffer[0] = elem;
  buffer[1] = elem;

  EXPECT_TRUE(buffer[0] == buffer[1]);
  EXPECT_TRUE(buffer[0] == elem);

  buffer[1] = Image::Buffer::Element(0xAA, 0xBB, 0xCC, 0xDD);
  EXPECT_FALSE(buffer[0] == buffer[1]);
}

TEST_F(BufferTest, BufferMoveAssignment) {
  Image::Buffer::Element elem(0xFF, 0x80, 0x40, 0xFF);
  buffer[0] = Image::Buffer::Element(0xFF, 0x80, 0x40, 0xFF);

  EXPECT_EQ(buffer[0][0], 0xFF);
  EXPECT_EQ(buffer[0][1], 0x80);
  EXPECT_EQ(buffer[0][2], 0x40);
  EXPECT_EQ(buffer[0][3], 0xFF);
}
