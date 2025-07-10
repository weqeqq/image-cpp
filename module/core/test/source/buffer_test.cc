
// #include <gtest/gtest.h>
#include <image/core/buffer.h>
// #include <sstream>

// #include <numeric>
// #include <iterator>

// TEST(Buffer, ConstructorAndDimensions) {
//     constexpr unsigned rows = 10;
//     constexpr unsigned cols = 20;
//     constexpr unsigned channels = 3;
//     Image::Buffer<Image::U8> buffer(rows, cols, channels);

//     EXPECT_EQ(buffer.RowCount(), rows);
//     EXPECT_EQ(buffer.ColumnCount(), cols);
//     EXPECT_EQ(buffer.ChannelCount(), channels);
//     EXPECT_EQ(buffer.Length(), rows * cols);
//     EXPECT_EQ(buffer.Count(), rows * cols * channels);
// }

// TEST(Buffer, ValueConstructor) {
//     constexpr unsigned rows = 5;
//     constexpr unsigned cols = 8;
//     constexpr unsigned channels = 1;
//     constexpr Image::U8 fill_value = 42;
//     Image::Buffer<Image::U8> buffer(rows, cols, channels, fill_value);

//     EXPECT_EQ(buffer.RowCount(), rows);
//     EXPECT_EQ(buffer.ColumnCount(), cols);
//     EXPECT_EQ(buffer.ChannelCount(), channels);

//     for (const auto& val : buffer.Contiguous()) {
//         EXPECT_EQ(val, fill_value);
//     }
// }

// TEST(Buffer, SubscriptOperator1D) {
//     constexpr unsigned rows = 2;
//     constexpr unsigned cols = 2;
//     constexpr unsigned channels = 3;
//     Image::Buffer<int> buffer(rows, cols, channels);

//     int counter = 0;
//     for (auto& val : buffer.Contiguous()) {
//         val = counter++;
//     }

//     // Non-const access and modification
//     auto* pixel0 = buffer[0]; // Pixel at (0, 0)
//     EXPECT_EQ(pixel0[0], 0);
//     EXPECT_EQ(pixel0[1], 1);
//     EXPECT_EQ(pixel0[2], 2);
//     pixel0[1] = 100;
//     EXPECT_EQ(buffer.Contiguous()[1], 100);

//     auto* pixel2 = buffer[2]; // Pixel at (1, 0)
//     EXPECT_EQ(pixel2[0], 6);
//     EXPECT_EQ(pixel2[1], 7);
//     EXPECT_EQ(pixel2[2], 8);

//     // Const access
//     const auto& const_buffer = buffer;
//     const auto* const_pixel1 = const_buffer[1]; // Pixel at (0, 1)
//     EXPECT_EQ(const_pixel1[0], 3);
//     EXPECT_EQ(const_pixel1[1], 4);
//     EXPECT_EQ(const_pixel1[2], 5);
// }

// TEST(Buffer, SubscriptOperator2D) {
//     constexpr unsigned rows = 2;
//     constexpr unsigned cols = 3;
//     constexpr unsigned channels = 2;
//     Image::Buffer<float> buffer(rows, cols, channels);

//     float counter = 0.0f;
//     for (auto& val : buffer.Contiguous()) {
//         val = counter++;
//     }

//     // Non-const access and modification
//     auto* pixel_0_1 = buffer[0, 1];
//     EXPECT_FLOAT_EQ(pixel_0_1[0], 2.0f);
//     EXPECT_FLOAT_EQ(pixel_0_1[1], 3.0f);
//     pixel_0_1[0] = -1.0f;
//     EXPECT_FLOAT_EQ(buffer.Contiguous()[2], -1.0f);

//     // Const access
//     const auto& const_buffer = buffer;
//     const auto* pixel_1_2 = const_buffer[1, 2];
//     EXPECT_FLOAT_EQ(pixel_1_2[0], 10.0f);
//     EXPECT_FLOAT_EQ(pixel_1_2[1], 11.0f);
// }

// TEST(Buffer, ContiguousSpan) {
//     constexpr unsigned rows = 4;
//     constexpr unsigned cols = 4;
//     constexpr unsigned channels = 1;
//     Image::Buffer<Image::U16> buffer(rows, cols, channels);

//     EXPECT_EQ(buffer.Contiguous().size(), buffer.Count());

//     std::iota(buffer.Contiguous().begin(), buffer.Contiguous().end(), 0);

//     const auto& const_buffer = buffer;
//     for (unsigned r = 0; r < rows; ++r) {
//         for (unsigned c = 0; c < cols; ++c) {
//           auto val = const_buffer[r, c][0];
//           EXPECT_EQ(val, r * cols + c);
//         }
//     }
// }

// TEST(Buffer, IteratorRangeFor) {
//     constexpr unsigned rows = 2;
//     constexpr unsigned cols = 3;
//     constexpr unsigned channels = 4;
//     Image::Buffer<Image::U8> buffer(rows, cols, channels);

//     std::iota(buffer.Contiguous().begin(), buffer.Contiguous().end(), 0);

//     // Non-const iteration
//     unsigned pixel_index = 0;
//     for (auto pixel_ptr : buffer) {
//         ASSERT_NE(pixel_ptr, nullptr);
//         for (unsigned ch = 0; ch < channels; ++ch) {
//             EXPECT_EQ(pixel_ptr[ch], pixel_index * channels + ch);
//         }
//         pixel_index++;
//     }
//     EXPECT_EQ(pixel_index, buffer.Length());

//     // Const iteration
//     const auto& const_buffer = buffer;
//     pixel_index = 0;
//     for (const auto pixel_ptr : const_buffer) {
//         ASSERT_NE(pixel_ptr, nullptr);
//         for (unsigned ch = 0; ch < channels; ++ch) {
//             EXPECT_EQ(pixel_ptr[ch], pixel_index * channels + ch);
//         }
//         pixel_index++;
//     }
//     EXPECT_EQ(pixel_index, buffer.Length());
// }

// TEST(Buffer, IteratorOperations) {
//     constexpr unsigned rows = 5;
//     constexpr unsigned cols = 10;
//     constexpr unsigned channels = 1;
//     Image::Buffer<int> buffer(rows, cols, channels);
//     std::iota(buffer.Contiguous().begin(), buffer.Contiguous().end(), 0);

//     auto it = buffer.begin();
//     auto end_it = buffer.end();

//     EXPECT_EQ(end_it - it, buffer.Length());
//     EXPECT_EQ(std::distance(it, end_it), buffer.Length());

//     EXPECT_EQ((*it)[0], 0);
//     it++;
//     EXPECT_EQ((*it)[0], 1);
//     ++it;
//     EXPECT_EQ((*it)[0], 2);
//     it--;
//     EXPECT_EQ((*it)[0], 1);
//     --it;
//     EXPECT_EQ((*it)[0], 0);

//     auto it5 = it + 5;
//     EXPECT_EQ((*it5)[0], 5);
//     auto it3 = it5 - 2;
//     EXPECT_EQ((*it3)[0], 3);

//     it += 10;
//     EXPECT_EQ((*it)[0], 10);
//     it -= 5;
//     EXPECT_EQ((*it)[0], 5);

//     EXPECT_EQ(it[3][0], 8);

//     auto it_copy = it;
//     EXPECT_TRUE(it == it_copy);
//     EXPECT_TRUE(it != it3);
//     EXPECT_TRUE(it3 < it);
//     EXPECT_TRUE(it > it3);
//     EXPECT_TRUE(it3 <= it);
//     EXPECT_TRUE(it <= it_copy);
//     EXPECT_TRUE(it >= it3);
//     EXPECT_TRUE(it >= it_copy);

//     // Dereference and modify
//     (*it)[0] = 99; // it points to pixel 5
//     EXPECT_EQ(buffer[5][0], 99);
// }

// TEST(Buffer, IteratorWithStruct) {
//     struct Pixel {
//         int r, g, b;
//     };
//     constexpr unsigned rows = 2;
//     constexpr unsigned cols = 2;
//     constexpr unsigned channels = 1;
//     Image::Buffer<Pixel> buffer(rows, cols, channels);

//     int counter = 0;
//     for (auto pixel_ptr : buffer) {
//         pixel_ptr->r = counter++;
//         pixel_ptr->g = counter++;
//         pixel_ptr->b = counter++;
//     }

//     counter = 0;
//     for (auto pixel_ptr : buffer) {
//         EXPECT_EQ(pixel_ptr->r, counter++);
//         EXPECT_EQ(pixel_ptr->g, counter++);
//         EXPECT_EQ(pixel_ptr->b, counter++);
//     }

//     auto it = buffer.begin();
//     it += 2; // Point to pixel at (1,0), index 2
//     EXPECT_EQ(it->r, 6);
//     EXPECT_EQ(it->g, 7);
//     EXPECT_EQ(it->b, 8);
// }
