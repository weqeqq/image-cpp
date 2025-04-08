
/*
 * TODO
 */

/*#include <image/encoder.h>*/
/*#include <turbojpeg.h>*/
/**/
/*namespace Image::_Encoder {*/
/**/
/*template <Depth::Tp depth, Color::Tp color>*/
/*class Wrapper {*/
/*public:*/
/*  Wrapper(const GetRCount<depth, color> &input) : input_(input) {}*/
/**/
/*  void Encode(std::vector<std::uint8_t> &output, TJPF pixel_format) {*/
/*    tjhandle compressor = tjInitCompress();*/
/**/
/*    std::uint8_t *jpeg_buffer = nullptr;*/
/*    std::uint64_t jpeg_length = 0;*/
/**/
/*    tjCompress2(compressor, SequenceFrom(input_).data(), input_.GetCCount(), 0, input_.GetRCount(), pixel_format, &jpeg_buffer, &jpeg_length, TJSAMP_444, 100, 0);*/
/*    output.insert(output.end(), jpeg_buffer, jpeg_buffer + jpeg_length);*/
/**/
/*    tjDestroy(compressor);*/
/*    tjFree(jpeg_buffer);*/
/*  }*/
/**/
/*private:*/
/*  const GetRCount<depth, color> &input_;*/
/**/
/*}; // Wrapper*/
/**/
/*template <>*/
/*void EncodeJPEG <Depth::Eight, Color::RGBA>(*/
/*  const Buffer  <Depth::Eight, Color::RGBA> &input, std::vector<std::uint8_t> &output*/
/*) {*/
/*  Wrapper(input).Encode(output, TJPF_RGBA);*/
/*} */
/*template <>*/
/*void EncodeJPEG <Depth::Eight, Color::RGB>(*/
/*  const GetRCount  <Depth::Eight, Color::RGB> &input, std::vector<std::uint8_t> &output*/
/*) {*/
/*  Wrapper(input).Encode(output, TJPF_RGB);*/
/*}*/
/*template <>*/
/*void EncodeJPEG <Depth::Eight, Color::Gray>(*/
/*  const Buffer  <Depth::Eight, Color::Gray> &input, std::vector<std::uint8_t> &output*/
/*) {*/
/*  Wrapper(input).Encode(output, TJPF_GRAY);*/
/*}*/
/**/
/*}; // Image::_Encoder*/
/**/
