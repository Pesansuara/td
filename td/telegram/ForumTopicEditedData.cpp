//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2022
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "td/telegram/ForumTopicEditedData.h"

#include "td/utils/logging.h"

namespace td {

td_api::object_ptr<td_api::forumTopicEditedData> ForumTopicEditedData::get_forum_topic_edited_data_object() const {
  return td_api::make_object<td_api::forumTopicEditedData>(title_, edit_icon_custom_emoji_id_,
                                                           icon_custom_emoji_id_.get(), edit_is_closed_, is_closed_);
}

StringBuilder &operator<<(StringBuilder &string_builder, const ForumTopicEditedData &topic_edited_data) {
  if (!topic_edited_data.title_.empty()) {
    string_builder << "set title to \"" << topic_edited_data.title_ << '"';
  }
  if (topic_edited_data.edit_icon_custom_emoji_id_) {
    string_builder << "set icon to " << topic_edited_data.icon_custom_emoji_id_;
  }
  if (topic_edited_data.edit_is_closed_) {
    string_builder << "set is_closed to " << topic_edited_data.is_closed_;
  }
  return string_builder;
}

}  // namespace td