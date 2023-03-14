#include "page/bitmap_page.h"

template<size_t PageSize>
bool BitmapPage<PageSize>::AllocatePage(uint32_t &page_offset) {
  if(page_allocated_ == (MAX_CHARS * 8)) return false;
  ++page_allocated_;
  page_offset = next_free_page_;

  bytes[page_offset/8] |= 0x01 << (page_offset%8);
  while(!IsPageFree(next_free_page_) && next_free_page_ <= MAX_CHARS*8){
    ++next_free_page_;
  }
  return true;
}

template<size_t PageSize>
bool BitmapPage<PageSize>::DeAllocatePage(uint32_t page_offset) {
  if(IsPageFree(page_offset)) return false;
  bytes[page_offset/8] &= ~(0x01 << (page_offset%8));
  --page_allocated_;
  next_free_page_ = page_offset < next_free_page_ ? page_offset : next_free_page_;
  return true;
}

// template<size_t PageSize>
// bool BitmapPage<PageSize>::IsPageFree(uint32_t page_offset) const {
//   return IsPageFreeLow(page_offset / 8, page_offset % 8);
// }

// template<size_t PageSize>
// bool BitmapPage<PageSize>::IsPageFreeLow(uint32_t byte_index, uint8_t bit_index) const {
//   // if(((byte_index<<3) + bit_index) >= MAX_CHARS*8) return false;
//   if(((bytes[byte_index]>>bit_index) & 0x01) == 0) return true;
//   return false;
// }
template<size_t PageSize>
bool BitmapPage<PageSize>::IsPageFree(uint32_t page_offset) const {
  if(IsPageFreeLow(page_offset/8, page_offset%8)) return true;
  return false;
}

template<size_t PageSize>
bool BitmapPage<PageSize>::IsPageFreeLow(uint32_t byte_index, uint8_t bit_index) const {
  if((bytes[byte_index]>>bit_index & 0x01) == 0) return true;
  return false;
}

template
class BitmapPage<64>;

template
class BitmapPage<128>;

template
class BitmapPage<256>;

template
class BitmapPage<512>;

template
class BitmapPage<1024>;

template
class BitmapPage<2048>;

template
class BitmapPage<4096>;