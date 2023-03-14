#include "buffer/lru_replacer.h"

LRUReplacer::LRUReplacer(size_t num_pages):num_pages_(num_pages) {
  lru_map_.reserve(this->num_pages_);
}

LRUReplacer::~LRUReplacer() = default;

bool LRUReplacer::Victim(frame_id_t *frame_id) {
  std::scoped_lock lock{mut};
  if(lru_list_.empty()) return false;
  *frame_id = lru_list_.back();
  lru_list_.pop_back();
  lru_map_.erase(*frame_id);
  return true;
}

void LRUReplacer::Pin(frame_id_t frame_id) {
  std::scoped_lock lock{mut};
  if(lru_map_.count(frame_id) == 0) return;
  
  auto a = lru_map_[frame_id];
  lru_list_.erase(a);
  lru_map_.erase(frame_id);
}

void LRUReplacer::Unpin(frame_id_t frame_id) {
  std::scoped_lock lock{mut};
  if(lru_map_.count(frame_id) != 0){
    return;
  }
  lru_list_.push_front(frame_id);
  lru_map_.emplace(frame_id, lru_list_.begin());
}

size_t LRUReplacer::Size() {
  return lru_list_.size();
}