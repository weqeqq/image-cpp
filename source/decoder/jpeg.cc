
/*
 * TODO
 */

/*#include <image/decoder.h>*/
/*#include <turbojpeg.h>*/
/**/
/*namespace Image::_Decoder {*/
/**/
/*class Wrapper {*/
/*public:*/
/*  Wrapper(const std::vector<std::uint8_t> &input) : input_(input) {}*/
/**/
/*  template <Depth::Tp depth, Color::Tp color>*/
/*  Buffer<depth, color> Decode(TJPF pixel_format) {*/
/*    std::int32_t row_count, column_count, subsamp;*/
/**/
/*    tjhandle decompressor = tjInitDecompress();*/
/*    if (tjDecompressHeader2(decompressor, const_cast<std::uint8_t *>(input_.data()), input_.size(), &column_count, &row_count, &subsamp)) {*/
/*      throw std::runtime_error("failed to decode header");*/
/*    }*/
/**/
/*    std::vector<std::uint8_t> buffer(row_count * column_count * tjPixelSize[pixel_format]);*/
/*    if (tjDecompress2(decompressor, const_cast<std::uint8_t *>(input_.data()), input_.size(), buffer.data(), column_count, 0, row_count, pixel_format, 0)) {*/
/*      throw std::runtime_error("failed to decode image");*/
/*    }*/
/**/
/*    tjDestroy(decompressor);*/
/*    return SequenceTo<depth, color>(buffer, row_count, column_count);*/
/*  }*/
/**/
/*private:*/
/*  const std::vector<std::uint8_t> &input_;*/
/*};*/
/**/
/*template <>*/
/*Buffer     <Depth::Eight, Color::RGBA> */
/*DecodeJPEG <Depth::Eight, Color::RGBA>(*/
/*  const std::vector<std::uint8_t> &input*/
/*) {*/
/*  return Wrapper(input).Decode<Depth::Eight, Color::RGBA>(TJPF_RGBA);*/
/*}*/
/*template <>*/
/*Buffer     <Depth::Eight, Color::RGB> */
/*DecodeJPEG <Depth::Eight, Color::RGB>(*/
/*  const std::vector<std::uint8_t> &input*/
/*) {*/
/*  return Wrapper(input).Decode<Depth::Eight, Color::RGB>(TJPF_RGB);*/
/*}*/
/*template <>*/
/*Buffer     <Depth::Eight, Color::Gray> */
/*DecodeJPEG <Depth::Eight, Color::Gray>(*/
/*  const std::vector<std::uint8_t> &input*/
/*) {*/
/*  return Wrapper(input).Decode<Depth::Eight, Color::Gray>(TJPF_GRAY);*/
/*}*/
/**/
/*}; // Image::_Decoder*/
