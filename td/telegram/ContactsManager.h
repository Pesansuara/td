//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2024
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "td/telegram/AccentColorId.h"
#include "td/telegram/AccessRights.h"
#include "td/telegram/BotCommand.h"
#include "td/telegram/BotMenuButton.h"
#include "td/telegram/ChannelId.h"
#include "td/telegram/ChannelType.h"
#include "td/telegram/ChatId.h"
#include "td/telegram/Contact.h"
#include "td/telegram/CustomEmojiId.h"
#include "td/telegram/DialogId.h"
#include "td/telegram/DialogInviteLink.h"
#include "td/telegram/DialogLocation.h"
#include "td/telegram/DialogParticipant.h"
#include "td/telegram/DialogParticipantFilter.h"
#include "td/telegram/EmojiStatus.h"
#include "td/telegram/files/FileId.h"
#include "td/telegram/files/FileSourceId.h"
#include "td/telegram/FolderId.h"
#include "td/telegram/Location.h"
#include "td/telegram/MessageFullId.h"
#include "td/telegram/MessageId.h"
#include "td/telegram/MessageTtl.h"
#include "td/telegram/net/DcId.h"
#include "td/telegram/Photo.h"
#include "td/telegram/PremiumGiftOption.h"
#include "td/telegram/PublicDialogType.h"
#include "td/telegram/QueryCombiner.h"
#include "td/telegram/QueryMerger.h"
#include "td/telegram/RestrictionReason.h"
#include "td/telegram/SecretChatId.h"
#include "td/telegram/StickerSetId.h"
#include "td/telegram/StoryId.h"
#include "td/telegram/SuggestedAction.h"
#include "td/telegram/td_api.h"
#include "td/telegram/telegram_api.h"
#include "td/telegram/UserId.h"
#include "td/telegram/Usernames.h"

#include "td/actor/actor.h"
#include "td/actor/MultiPromise.h"
#include "td/actor/MultiTimeout.h"

#include "td/utils/common.h"
#include "td/utils/FlatHashMap.h"
#include "td/utils/FlatHashSet.h"
#include "td/utils/HashTableUtils.h"
#include "td/utils/Hints.h"
#include "td/utils/Promise.h"
#include "td/utils/Status.h"
#include "td/utils/StringBuilder.h"
#include "td/utils/Time.h"
#include "td/utils/WaitFreeHashMap.h"
#include "td/utils/WaitFreeHashSet.h"

#include <functional>
#include <memory>
#include <utility>

namespace td {

struct BinlogEvent;

class ChannelParticipantFilter;

struct MinChannel;

class Td;

class ContactsManager final : public Actor {
 public:
  ContactsManager(Td *td, ActorShared<> parent);
  ContactsManager(const ContactsManager &) = delete;
  ContactsManager &operator=(const ContactsManager &) = delete;
  ContactsManager(ContactsManager &&) = delete;
  ContactsManager &operator=(ContactsManager &&) = delete;
  ~ContactsManager() final;

  static UserId load_my_id();

  static UserId get_user_id(const tl_object_ptr<telegram_api::User> &user);
  static ChatId get_chat_id(const tl_object_ptr<telegram_api::Chat> &chat);
  static ChannelId get_channel_id(const tl_object_ptr<telegram_api::Chat> &chat);
  static DialogId get_dialog_id(const tl_object_ptr<telegram_api::Chat> &chat);

  vector<ChannelId> get_channel_ids(vector<tl_object_ptr<telegram_api::Chat>> &&chats, const char *source);

  Result<tl_object_ptr<telegram_api::InputUser>> get_input_user(UserId user_id) const;

  tl_object_ptr<telegram_api::InputUser> get_input_user_force(UserId user_id) const;

  // TODO get_input_chat ???

  tl_object_ptr<telegram_api::InputChannel> get_input_channel(ChannelId channel_id) const;

  tl_object_ptr<telegram_api::InputPeer> get_input_peer_user(UserId user_id, AccessRights access_rights) const;
  bool have_input_peer_user(UserId user_id, AccessRights access_rights) const;

  tl_object_ptr<telegram_api::InputPeer> get_input_peer_chat(ChatId chat_id, AccessRights access_rights) const;
  bool have_input_peer_chat(ChatId chat_id, AccessRights access_rights) const;

  tl_object_ptr<telegram_api::InputPeer> get_input_peer_channel(ChannelId channel_id, AccessRights access_rights) const;
  bool have_input_peer_channel(ChannelId channel_id, AccessRights access_rights) const;

  tl_object_ptr<telegram_api::inputEncryptedChat> get_input_encrypted_chat(SecretChatId secret_chat_id,
                                                                           AccessRights access_rights) const;
  bool have_input_encrypted_peer(SecretChatId secret_chat_id, AccessRights access_rights) const;

  const DialogPhoto *get_user_dialog_photo(UserId user_id);
  const DialogPhoto *get_chat_dialog_photo(ChatId chat_id) const;
  const DialogPhoto *get_channel_dialog_photo(ChannelId channel_id) const;
  const DialogPhoto *get_secret_chat_dialog_photo(SecretChatId secret_chat_id);

  AccentColorId get_channel_accent_color_id(ChannelId channel_id) const;

  int32 get_user_accent_color_id_object(UserId user_id) const;
  int32 get_chat_accent_color_id_object(ChatId chat_id) const;
  int32 get_channel_accent_color_id_object(ChannelId channel_id) const;
  int32 get_secret_chat_accent_color_id_object(SecretChatId secret_chat_id) const;

  CustomEmojiId get_user_background_custom_emoji_id(UserId user_id) const;
  CustomEmojiId get_chat_background_custom_emoji_id(ChatId chat_id) const;
  CustomEmojiId get_channel_background_custom_emoji_id(ChannelId channel_id) const;
  CustomEmojiId get_secret_chat_background_custom_emoji_id(SecretChatId secret_chat_id) const;

  int32 get_user_profile_accent_color_id_object(UserId user_id) const;
  int32 get_chat_profile_accent_color_id_object(ChatId chat_id) const;
  int32 get_channel_profile_accent_color_id_object(ChannelId channel_id) const;
  int32 get_secret_chat_profile_accent_color_id_object(SecretChatId secret_chat_id) const;

  CustomEmojiId get_user_profile_background_custom_emoji_id(UserId user_id) const;
  CustomEmojiId get_chat_profile_background_custom_emoji_id(ChatId chat_id) const;
  CustomEmojiId get_channel_profile_background_custom_emoji_id(ChannelId channel_id) const;
  CustomEmojiId get_secret_chat_profile_background_custom_emoji_id(SecretChatId secret_chat_id) const;

  string get_user_title(UserId user_id) const;
  string get_chat_title(ChatId chat_id) const;
  string get_channel_title(ChannelId channel_id) const;
  string get_secret_chat_title(SecretChatId secret_chat_id) const;

  RestrictedRights get_user_default_permissions(UserId user_id) const;
  RestrictedRights get_chat_default_permissions(ChatId chat_id) const;
  RestrictedRights get_channel_default_permissions(ChannelId channel_id) const;
  RestrictedRights get_secret_chat_default_permissions(SecretChatId secret_chat_id) const;

  td_api::object_ptr<td_api::emojiStatus> get_user_emoji_status_object(UserId user_id) const;
  td_api::object_ptr<td_api::emojiStatus> get_chat_emoji_status_object(ChatId chat_id) const;
  td_api::object_ptr<td_api::emojiStatus> get_channel_emoji_status_object(ChannelId channel_id) const;
  td_api::object_ptr<td_api::emojiStatus> get_secret_chat_emoji_status_object(SecretChatId secret_chat_id) const;

  bool get_chat_has_protected_content(ChatId chat_id) const;
  bool get_channel_has_protected_content(ChannelId channel_id) const;

  bool get_user_stories_hidden(UserId user_id) const;
  bool get_channel_stories_hidden(ChannelId channel_id) const;

  string get_user_private_forward_name(UserId user_id);

  bool get_user_voice_messages_forbidden(UserId user_id) const;

  bool get_user_read_dates_private(UserId user_id);

  string get_dialog_about(DialogId dialog_id);

  string get_dialog_search_text(DialogId dialog_id) const;

  void for_each_secret_chat_with_user(UserId user_id, const std::function<void(SecretChatId)> &f);

  string get_user_first_username(UserId user_id) const;
  string get_channel_first_username(ChannelId channel_id) const;
  string get_channel_editable_username(ChannelId channel_id) const;

  int32 get_secret_chat_date(SecretChatId secret_chat_id) const;
  int32 get_secret_chat_ttl(SecretChatId secret_chat_id) const;
  UserId get_secret_chat_user_id(SecretChatId secret_chat_id) const;
  bool get_secret_chat_is_outbound(SecretChatId secret_chat_id) const;
  SecretChatState get_secret_chat_state(SecretChatId secret_chat_id) const;
  int32 get_secret_chat_layer(SecretChatId secret_chat_id) const;
  FolderId get_secret_chat_initial_folder_id(SecretChatId secret_chat_id) const;

  void can_send_message_to_user(UserId user_id, bool force,
                                Promise<td_api::object_ptr<td_api::CanSendMessageToUserResult>> &&promise);

  void allow_send_message_to_user(UserId user_id);

  void on_imported_contacts(int64 random_id, Result<tl_object_ptr<telegram_api::contacts_importedContacts>> result);

  void on_deleted_contacts(const vector<UserId> &deleted_contact_user_ids);

  void on_get_contacts(tl_object_ptr<telegram_api::contacts_Contacts> &&new_contacts);

  void on_get_contacts_failed(Status error);

  void on_get_contacts_statuses(vector<tl_object_ptr<telegram_api::contactStatus>> &&statuses);

  void reload_contacts(bool force);

  void on_get_user(tl_object_ptr<telegram_api::User> &&user, const char *source);
  void on_get_users(vector<tl_object_ptr<telegram_api::User>> &&users, const char *source);

  void on_get_is_premium_required_to_contact_users(vector<UserId> &&user_ids, vector<bool> &&is_premium_required,
                                                   Promise<Unit> &&promise);

  void on_binlog_user_event(BinlogEvent &&event);
  void on_binlog_chat_event(BinlogEvent &&event);
  void on_binlog_channel_event(BinlogEvent &&event);
  void on_binlog_secret_chat_event(BinlogEvent &&event);

  void on_get_user_full(tl_object_ptr<telegram_api::userFull> &&user);

  void on_get_user_photos(UserId user_id, int32 offset, int32 limit, int32 total_count,
                          vector<tl_object_ptr<telegram_api::Photo>> photos);

  void on_get_chat(tl_object_ptr<telegram_api::Chat> &&chat, const char *source);
  void on_get_chats(vector<tl_object_ptr<telegram_api::Chat>> &&chats, const char *source);

  void on_get_chat_full(tl_object_ptr<telegram_api::ChatFull> &&chat_full, Promise<Unit> &&promise);
  void on_get_chat_full_failed(ChatId chat_id);
  void on_get_channel_full_failed(ChannelId channel_id);

  void on_update_profile_success(int32 flags, const string &first_name, const string &last_name, const string &about);
  void on_update_accent_color_success(bool for_profile, AccentColorId accent_color_id,
                                      CustomEmojiId background_custom_emoji_id);

  void on_update_user_name(UserId user_id, string &&first_name, string &&last_name, Usernames &&usernames);
  void on_update_user_phone_number(UserId user_id, string &&phone_number);
  void on_update_user_emoji_status(UserId user_id, tl_object_ptr<telegram_api::EmojiStatus> &&emoji_status);
  void on_update_user_story_ids(UserId user_id, StoryId max_active_story_id, StoryId max_read_story_id);
  void on_update_user_max_read_story_id(UserId user_id, StoryId max_read_story_id);
  void on_update_user_stories_hidden(UserId user_id, bool stories_hidden);
  void on_update_user_online(UserId user_id, tl_object_ptr<telegram_api::UserStatus> &&status);
  void on_update_user_local_was_online(UserId user_id, int32 local_was_online);
  // use on_update_dialog_is_blocked instead
  void on_update_user_is_blocked(UserId user_id, bool is_blocked, bool is_blocked_for_stories);
  void on_update_user_has_pinned_stories(UserId user_id, bool has_pinned_stories);
  void on_update_user_common_chat_count(UserId user_id, int32 common_chat_count);
  void on_update_user_need_phone_number_privacy_exception(UserId user_id, bool need_phone_number_privacy_exception);
  void on_update_user_wallpaper_overridden(UserId user_id, bool wallpaper_overridden);

  void on_set_profile_photo(UserId user_id, tl_object_ptr<telegram_api::photos_photo> &&photo, bool is_fallback,
                            int64 old_photo_id, Promise<Unit> &&promise);

  void on_delete_profile_photo(int64 profile_photo_id, Promise<Unit> promise);

  void on_ignored_restriction_reasons_changed();

  void on_get_chat_participants(tl_object_ptr<telegram_api::ChatParticipants> &&participants, bool from_update);
  void on_update_chat_add_user(ChatId chat_id, UserId inviter_user_id, UserId user_id, int32 date, int32 version);
  void on_update_chat_description(ChatId chat_id, string &&description);
  void on_update_chat_edit_administrator(ChatId chat_id, UserId user_id, bool is_administrator, int32 version);
  void on_update_chat_delete_user(ChatId chat_id, UserId user_id, int32 version);
  void on_update_chat_default_permissions(ChatId chat_id, RestrictedRights default_permissions, int32 version);
  void on_update_chat_pinned_message(ChatId chat_id, MessageId pinned_message_id, int32 version);

  void on_update_channel_participant_count(ChannelId channel_id, int32 participant_count);
  void on_update_channel_editable_username(ChannelId channel_id, string &&username);
  void on_update_channel_usernames(ChannelId channel_id, Usernames &&usernames);
  void on_update_channel_story_ids(ChannelId channel_id, StoryId max_active_story_id, StoryId max_read_story_id);
  void on_update_channel_max_read_story_id(ChannelId channel_id, StoryId max_read_story_id);
  void on_update_channel_stories_hidden(ChannelId channel_id, bool stories_hidden);
  void on_update_channel_description(ChannelId channel_id, string &&description);
  void on_update_channel_sticker_set(ChannelId channel_id, StickerSetId sticker_set_id);
  void on_update_channel_emoji_sticker_set(ChannelId channel_id, StickerSetId sticker_set_id);
  void on_update_channel_linked_channel_id(ChannelId channel_id, ChannelId group_channel_id);
  void on_update_channel_location(ChannelId channel_id, const DialogLocation &location);
  void on_update_channel_slow_mode_delay(ChannelId channel_id, int32 slow_mode_delay, Promise<Unit> &&promise);
  void on_update_channel_slow_mode_next_send_date(ChannelId channel_id, int32 slow_mode_next_send_date);
  void on_update_channel_is_all_history_available(ChannelId channel_id, bool is_all_history_available,
                                                  Promise<Unit> &&promise);
  void on_update_channel_has_hidden_participants(ChannelId channel_id, bool has_hidden_participants,
                                                 Promise<Unit> &&promise);
  void on_update_channel_has_aggressive_anti_spam_enabled(ChannelId channel_id, bool has_aggressive_anti_spam_enabled,
                                                          Promise<Unit> &&promise);
  void on_update_channel_has_pinned_stories(ChannelId channel_id, bool has_pinned_stories);
  void on_update_channel_default_permissions(ChannelId channel_id, RestrictedRights default_permissions);
  void on_update_channel_administrator_count(ChannelId channel_id, int32 administrator_count);

  int32 on_update_peer_located(vector<tl_object_ptr<telegram_api::PeerLocated>> &&peers, bool from_update);

  void on_update_bot_commands(DialogId dialog_id, UserId bot_user_id,
                              vector<tl_object_ptr<telegram_api::botCommand>> &&bot_commands);

  void on_update_bot_menu_button(UserId bot_user_id, tl_object_ptr<telegram_api::BotMenuButton> &&bot_menu_button);

  void speculative_add_channel_participants(ChannelId channel_id, const vector<UserId> &added_user_ids,
                                            UserId inviter_user_id, int32 date, bool by_me);

  void speculative_delete_channel_participant(ChannelId channel_id, UserId deleted_user_id, bool by_me);

  void invalidate_channel_full(ChannelId channel_id, bool need_drop_slow_mode_delay, const char *source);

  bool on_get_channel_error(ChannelId channel_id, const Status &status, const char *source);

  void on_get_permanent_dialog_invite_link(DialogId dialog_id, const DialogInviteLink &invite_link);

  void on_get_created_public_channels(PublicDialogType type, vector<tl_object_ptr<telegram_api::Chat>> &&chats);

  void on_get_dialogs_for_discussion(vector<tl_object_ptr<telegram_api::Chat>> &&chats);

  void on_get_inactive_channels(vector<tl_object_ptr<telegram_api::Chat>> &&chats, Promise<Unit> &&promise);

  void remove_inactive_channel(ChannelId channel_id);

  void register_message_users(MessageFullId message_full_id, vector<UserId> user_ids);

  void register_message_channels(MessageFullId message_full_id, vector<ChannelId> channel_ids);

  void unregister_message_users(MessageFullId message_full_id, vector<UserId> user_ids);

  void unregister_message_channels(MessageFullId message_full_id, vector<ChannelId> channel_ids);

  bool can_use_premium_custom_emoji(DialogId dialog_id) const;

  UserId get_my_id() const;

  void set_my_online_status(bool is_online, bool send_update, bool is_local);

  struct MyOnlineStatusInfo {
    bool is_online_local = false;
    bool is_online_remote = false;
    int32 was_online_local = 0;
    int32 was_online_remote = 0;
  };

  MyOnlineStatusInfo get_my_online_status() const;

  static UserId get_service_notifications_user_id();

  UserId add_service_notifications_user();

  static UserId get_replies_bot_user_id();

  static UserId get_anonymous_bot_user_id();

  static UserId get_channel_bot_user_id();

  static UserId get_anti_spam_bot_user_id();

  UserId add_anonymous_bot_user();

  UserId add_channel_bot_user();

  static ChannelId get_unsupported_channel_id();

  void on_update_username_is_active(UserId user_id, string &&username, bool is_active, Promise<Unit> &&promise);

  void on_update_active_usernames_order(UserId user_id, vector<string> &&usernames, Promise<Unit> &&promise);

  void on_update_channel_username_is_active(ChannelId channel_id, string &&username, bool is_active,
                                            Promise<Unit> &&promise);

  void on_deactivate_channel_usernames(ChannelId channel_id, Promise<Unit> &&promise);

  void on_update_channel_active_usernames_order(ChannelId channel_id, vector<string> &&usernames,
                                                Promise<Unit> &&promise);

  void on_update_online_status_privacy();

  void on_update_phone_number_privacy();

  void invalidate_user_full(UserId user_id);

  void add_contact(Contact contact, bool share_phone_number, Promise<Unit> &&promise);

  std::pair<vector<UserId>, vector<int32>> import_contacts(const vector<Contact> &contacts, int64 &random_id,
                                                           Promise<Unit> &&promise);

  std::pair<int32, vector<UserId>> search_contacts(const string &query, int32 limit, Promise<Unit> &&promise);

  void remove_contacts(const vector<UserId> &user_ids, Promise<Unit> &&promise);

  void remove_contacts_by_phone_number(vector<string> user_phone_numbers, vector<UserId> user_ids,
                                       Promise<Unit> &&promise);

  int32 get_imported_contact_count(Promise<Unit> &&promise);

  std::pair<vector<UserId>, vector<int32>> change_imported_contacts(vector<Contact> &contacts, int64 &random_id,
                                                                    Promise<Unit> &&promise);

  void clear_imported_contacts(Promise<Unit> &&promise);

  void on_update_contacts_reset();

  vector<UserId> get_close_friends(Promise<Unit> &&promise);

  void set_close_friends(vector<UserId> user_ids, Promise<Unit> &&promise);

  void on_set_close_friends(const vector<UserId> &user_ids, Promise<Unit> &&promise);

  UserId search_user_by_phone_number(string phone_number, Promise<Unit> &&promise);

  void on_resolved_phone_number(const string &phone_number, UserId user_id);

  void share_phone_number(UserId user_id, Promise<Unit> &&promise);

  void search_dialogs_nearby(const Location &location, Promise<td_api::object_ptr<td_api::chatsNearby>> &&promise);

  void set_location(const Location &location, Promise<Unit> &&promise);

  static void set_location_visibility(Td *td);

  void get_is_location_visible(Promise<Unit> &&promise);

  void register_suggested_profile_photo(const Photo &photo);

  FileId get_profile_photo_file_id(int64 photo_id) const;

  void set_bot_profile_photo(UserId bot_user_id, const td_api::object_ptr<td_api::InputChatPhoto> &input_photo,
                             Promise<Unit> &&promise);

  void set_profile_photo(const td_api::object_ptr<td_api::InputChatPhoto> &input_photo, bool is_fallback,
                         Promise<Unit> &&promise);

  void set_user_profile_photo(UserId user_id, const td_api::object_ptr<td_api::InputChatPhoto> &input_photo,
                              bool only_suggest, Promise<Unit> &&promise);

  void send_update_profile_photo_query(UserId user_id, FileId file_id, int64 old_photo_id, bool is_fallback,
                                       Promise<Unit> &&promise);

  void delete_profile_photo(int64 profile_photo_id, bool is_recursive, Promise<Unit> &&promise);

  void set_accent_color(AccentColorId accent_color_id, CustomEmojiId background_custom_emoji_id,
                        Promise<Unit> &&promise);

  void set_profile_accent_color(AccentColorId accent_color_id, CustomEmojiId background_custom_emoji_id,
                                Promise<Unit> &&promise);

  void set_name(const string &first_name, const string &last_name, Promise<Unit> &&promise);

  void set_bio(const string &bio, Promise<Unit> &&promise);

  void set_username(const string &username, Promise<Unit> &&promise);

  void toggle_username_is_active(string &&username, bool is_active, Promise<Unit> &&promise);

  void reorder_usernames(vector<string> &&usernames, Promise<Unit> &&promise);

  void toggle_bot_username_is_active(UserId bot_user_id, string &&username, bool is_active, Promise<Unit> &&promise);

  void reorder_bot_usernames(UserId bot_user_id, vector<string> &&usernames, Promise<Unit> &&promise);

  void set_emoji_status(const EmojiStatus &emoji_status, Promise<Unit> &&promise);

  void set_chat_description(ChatId chat_id, const string &description, Promise<Unit> &&promise);

  void set_channel_username(ChannelId channel_id, const string &username, Promise<Unit> &&promise);

  void toggle_channel_username_is_active(ChannelId channel_id, string &&username, bool is_active,
                                         Promise<Unit> &&promise);

  void disable_all_channel_usernames(ChannelId channel_id, Promise<Unit> &&promise);

  void reorder_channel_usernames(ChannelId channel_id, vector<string> &&usernames, Promise<Unit> &&promise);

  void set_channel_accent_color(ChannelId channel_id, AccentColorId accent_color_id,
                                CustomEmojiId background_custom_emoji_id, Promise<Unit> &&promise);

  void set_channel_profile_accent_color(ChannelId channel_id, AccentColorId profile_accent_color_id,
                                        CustomEmojiId profile_background_custom_emoji_id, Promise<Unit> &&promise);

  void set_channel_emoji_status(ChannelId channel_id, const EmojiStatus &emoji_status, Promise<Unit> &&promise);

  void set_channel_sticker_set(ChannelId channel_id, StickerSetId sticker_set_id, Promise<Unit> &&promise);

  void set_channel_emoji_sticker_set(ChannelId channel_id, StickerSetId sticker_set_id, Promise<Unit> &&promise);

  void toggle_channel_sign_messages(ChannelId channel_id, bool sign_messages, Promise<Unit> &&promise);

  void toggle_channel_join_to_send(ChannelId channel_id, bool joint_to_send, Promise<Unit> &&promise);

  void toggle_channel_join_request(ChannelId channel_id, bool join_request, Promise<Unit> &&promise);

  void toggle_channel_is_all_history_available(ChannelId channel_id, bool is_all_history_available,
                                               Promise<Unit> &&promise);

  void toggle_channel_has_hidden_participants(ChannelId channel_id, bool has_hidden_participants,
                                              Promise<Unit> &&promise);

  void toggle_channel_has_aggressive_anti_spam_enabled(ChannelId channel_id, bool has_aggressive_anti_spam_enabled,
                                                       Promise<Unit> &&promise);

  void toggle_channel_is_forum(ChannelId channel_id, bool is_forum, Promise<Unit> &&promise);

  void convert_channel_to_gigagroup(ChannelId channel_id, Promise<Unit> &&promise);

  void set_channel_description(ChannelId channel_id, const string &description, Promise<Unit> &&promise);

  void set_channel_discussion_group(DialogId dialog_id, DialogId discussion_dialog_id, Promise<Unit> &&promise);

  void set_channel_location(DialogId dialog_id, const DialogLocation &location, Promise<Unit> &&promise);

  void set_channel_slow_mode_delay(DialogId dialog_id, int32 slow_mode_delay, Promise<Unit> &&promise);

  void report_channel_spam(ChannelId channel_id, const vector<MessageId> &message_ids, Promise<Unit> &&promise);

  void report_channel_anti_spam_false_positive(ChannelId channel_id, MessageId message_id, Promise<Unit> &&promise);

  void delete_chat(ChatId chat_id, Promise<Unit> &&promise);

  void delete_channel(ChannelId channel_id, Promise<Unit> &&promise);

  void get_channel_statistics_dc_id(DialogId dialog_id, bool for_full_statistics, Promise<DcId> &&promise);

  bool can_get_channel_message_statistics(DialogId dialog_id) const;

  bool can_get_channel_story_statistics(DialogId dialog_id) const;

  struct CanTransferOwnershipResult {
    enum class Type : uint8 { Ok, PasswordNeeded, PasswordTooFresh, SessionTooFresh };
    Type type = Type::Ok;
    int32 retry_after = 0;
  };
  void can_transfer_ownership(Promise<CanTransferOwnershipResult> &&promise);

  static td_api::object_ptr<td_api::CanTransferOwnershipResult> get_can_transfer_ownership_result_object(
      CanTransferOwnershipResult result);

  void transfer_dialog_ownership(DialogId dialog_id, UserId user_id, const string &password, Promise<Unit> &&promise);

  void migrate_dialog_to_megagroup(DialogId dialog_id, Promise<td_api::object_ptr<td_api::chat>> &&promise);

  void get_channel_recommendations(DialogId dialog_id, bool return_local,
                                   Promise<td_api::object_ptr<td_api::chats>> &&chats_promise,
                                   Promise<td_api::object_ptr<td_api::count>> &&count_promise);

  void get_created_public_dialogs(PublicDialogType type, Promise<td_api::object_ptr<td_api::chats>> &&promise,
                                  bool from_binlog);

  void open_channel_recommended_channel(DialogId dialog_id, DialogId opened_dialog_id, Promise<Unit> &&promise);

  void check_created_public_dialogs_limit(PublicDialogType type, Promise<Unit> &&promise);

  void reload_created_public_dialogs(PublicDialogType type, Promise<td_api::object_ptr<td_api::chats>> &&promise);

  vector<DialogId> get_dialogs_for_discussion(Promise<Unit> &&promise);

  vector<DialogId> get_inactive_channels(Promise<Unit> &&promise);

  void dismiss_dialog_suggested_action(SuggestedAction action, Promise<Unit> &&promise);

  bool is_user_contact(UserId user_id, bool is_mutual = false) const;

  bool is_user_premium(UserId user_id) const;

  bool is_user_deleted(UserId user_id) const;

  bool is_user_support(UserId user_id) const;

  bool is_user_bot(UserId user_id) const;

  struct BotData {
    string username;
    bool can_be_edited;
    bool can_join_groups;
    bool can_read_all_group_messages;
    bool is_inline;
    bool need_location;
    bool can_be_added_to_attach_menu;
  };
  Result<BotData> get_bot_data(UserId user_id) const TD_WARN_UNUSED_RESULT;

  bool is_user_online(UserId user_id, int32 tolerance = 0, int32 unix_time = 07) const;

  bool is_user_status_exact(UserId user_id) const;

  bool can_report_user(UserId user_id) const;

  bool have_user(UserId user_id) const;
  bool have_min_user(UserId user_id) const;
  bool have_user_force(UserId user_id, const char *source);

  bool is_dialog_info_received_from_server(DialogId dialog_id) const;

  static void send_get_me_query(Td *td, Promise<Unit> &&promise);
  UserId get_me(Promise<Unit> &&promise);
  bool get_user(UserId user_id, int left_tries, Promise<Unit> &&promise);
  void reload_user(UserId user_id, Promise<Unit> &&promise, const char *source);
  void load_user_full(UserId user_id, bool force, Promise<Unit> &&promise, const char *source);
  FileSourceId get_user_full_file_source_id(UserId user_id);
  void reload_user_full(UserId user_id, Promise<Unit> &&promise, const char *source);

  void get_user_profile_photos(UserId user_id, int32 offset, int32 limit,
                               Promise<td_api::object_ptr<td_api::chatPhotos>> &&promise);
  void reload_user_profile_photo(UserId user_id, int64 photo_id, Promise<Unit> &&promise);
  FileSourceId get_user_profile_photo_file_source_id(UserId user_id, int64 photo_id);

  void create_new_chat(const vector<UserId> &user_ids, const string &title, MessageTtl message_ttl,
                       Promise<td_api::object_ptr<td_api::chat>> &&promise);

  bool have_chat(ChatId chat_id) const;
  bool have_chat_force(ChatId chat_id, const char *source);
  bool get_chat(ChatId chat_id, int left_tries, Promise<Unit> &&promise);
  void reload_chat(ChatId chat_id, Promise<Unit> &&promise, const char *source);
  void load_chat_full(ChatId chat_id, bool force, Promise<Unit> &&promise, const char *source);
  FileSourceId get_chat_full_file_source_id(ChatId chat_id);
  void reload_chat_full(ChatId chat_id, Promise<Unit> &&promise, const char *source);

  int32 get_chat_date(ChatId chat_id) const;
  int32 get_chat_participant_count(ChatId chat_id) const;
  bool get_chat_is_active(ChatId chat_id) const;
  ChannelId get_chat_migrated_to_channel_id(ChatId chat_id) const;
  DialogParticipantStatus get_chat_status(ChatId chat_id) const;
  DialogParticipantStatus get_chat_permissions(ChatId chat_id) const;
  bool is_appointed_chat_administrator(ChatId chat_id) const;

  void create_new_channel(const string &title, bool is_forum, bool is_megagroup, const string &description,
                          const DialogLocation &location, bool for_import, MessageTtl message_ttl,
                          Promise<td_api::object_ptr<td_api::chat>> &&promise);

  bool have_min_channel(ChannelId channel_id) const;
  const MinChannel *get_min_channel(ChannelId channel_id) const;
  void add_min_channel(ChannelId channel_id, const MinChannel &min_channel);

  bool have_channel(ChannelId channel_id) const;
  bool have_channel_force(ChannelId channel_id, const char *source);
  bool get_channel(ChannelId channel_id, int left_tries, Promise<Unit> &&promise);
  void reload_channel(ChannelId channel_id, Promise<Unit> &&promise, const char *source);
  void load_channel_full(ChannelId channel_id, bool force, Promise<Unit> &&promise, const char *source);
  FileSourceId get_channel_full_file_source_id(ChannelId channel_id);
  void reload_channel_full(ChannelId channel_id, Promise<Unit> &&promise, const char *source);

  bool is_channel_public(ChannelId channel_id) const;

  void create_new_secret_chat(UserId user_id, Promise<td_api::object_ptr<td_api::chat>> &&promise);

  bool have_secret_chat(SecretChatId secret_chat_id) const;
  bool have_secret_chat_force(SecretChatId secret_chat_id, const char *source);
  bool get_secret_chat(SecretChatId secret_chat_id, bool force, Promise<Unit> &&promise);
  bool get_secret_chat_full(SecretChatId secret_chat_id, Promise<Unit> &&promise);

  ChannelType get_channel_type(ChannelId channel_id) const;
  bool is_broadcast_channel(ChannelId channel_id) const;
  bool is_megagroup_channel(ChannelId channel_id) const;
  bool is_forum_channel(ChannelId channel_id) const;
  int32 get_channel_date(ChannelId channel_id) const;
  DialogParticipantStatus get_channel_status(ChannelId channel_id) const;
  DialogParticipantStatus get_channel_permissions(ChannelId channel_id) const;
  bool get_channel_is_verified(ChannelId channel_id) const;
  bool get_channel_is_scam(ChannelId channel_id) const;
  bool get_channel_is_fake(ChannelId channel_id) const;
  int32 get_channel_participant_count(ChannelId channel_id) const;
  bool get_channel_sign_messages(ChannelId channel_id) const;
  bool get_channel_has_linked_channel(ChannelId channel_id) const;
  bool get_channel_join_request(ChannelId channel_id) const;
  bool get_channel_can_be_deleted(ChannelId channel_id) const;
  ChannelId get_channel_linked_channel_id(ChannelId channel_id, const char *source);
  int32 get_channel_slow_mode_delay(ChannelId channel_id, const char *source);
  bool get_channel_effective_has_hidden_participants(ChannelId channel_id, const char *source);
  int32 get_channel_my_boost_count(ChannelId channel_id);

  void add_chat_participant(ChatId chat_id, UserId user_id, int32 forward_limit, Promise<Unit> &&promise);

  void set_chat_participant_status(ChatId chat_id, UserId user_id, DialogParticipantStatus status,
                                   Promise<Unit> &&promise);

  void delete_chat_participant(ChatId chat_id, UserId user_id, bool revoke_messages, Promise<Unit> &&promise);

  void get_chat_participant(ChatId chat_id, UserId user_id, Promise<DialogParticipant> &&promise);

  void speculative_add_channel_user(ChannelId channel_id, UserId user_id, const DialogParticipantStatus &new_status,
                                    const DialogParticipantStatus &old_status);

  void search_dialog_participants(DialogId dialog_id, const string &query, int32 limit, DialogParticipantFilter filter,
                                  Promise<DialogParticipants> &&promise);

  void get_channel_participants(ChannelId channel_id, tl_object_ptr<td_api::SupergroupMembersFilter> &&filter,
                                string additional_query, int32 offset, int32 limit, int32 additional_limit,
                                Promise<DialogParticipants> &&promise);

  int64 get_user_id_object(UserId user_id, const char *source) const;

  tl_object_ptr<td_api::user> get_user_object(UserId user_id) const;

  vector<int64> get_user_ids_object(const vector<UserId> &user_ids, const char *source) const;

  tl_object_ptr<td_api::users> get_users_object(int32 total_count, const vector<UserId> &user_ids) const;

  tl_object_ptr<td_api::userFullInfo> get_user_full_info_object(UserId user_id) const;

  int64 get_basic_group_id_object(ChatId chat_id, const char *source) const;

  tl_object_ptr<td_api::basicGroup> get_basic_group_object(ChatId chat_id);

  tl_object_ptr<td_api::basicGroupFullInfo> get_basic_group_full_info_object(ChatId chat_id) const;

  int64 get_supergroup_id_object(ChannelId channel_id, const char *source) const;

  tl_object_ptr<td_api::supergroup> get_supergroup_object(ChannelId channel_id) const;

  tl_object_ptr<td_api::supergroupFullInfo> get_supergroup_full_info_object(ChannelId channel_id) const;

  int32 get_secret_chat_id_object(SecretChatId secret_chat_id, const char *source) const;

  tl_object_ptr<td_api::secretChat> get_secret_chat_object(SecretChatId secret_chat_id);

  void on_update_secret_chat(SecretChatId secret_chat_id, int64 access_hash, UserId user_id, SecretChatState state,
                             bool is_outbound, int32 ttl, int32 date, string key_hash, int32 layer,
                             FolderId initial_folder_id);

  tl_object_ptr<td_api::chatMember> get_chat_member_object(const DialogParticipant &dialog_participant,
                                                           const char *source) const;

  void get_support_user(Promise<td_api::object_ptr<td_api::user>> &&promise);

  void on_view_dialog_active_stories(vector<DialogId> dialog_ids);

  void on_get_dialog_max_active_story_ids(const vector<DialogId> &dialog_ids, const vector<int32> &max_story_ids);

  void repair_chat_participants(ChatId chat_id);

  void get_current_state(vector<td_api::object_ptr<td_api::Update>> &updates) const;

 private:
  struct User {
    string first_name;
    string last_name;
    Usernames usernames;
    string phone_number;
    int64 access_hash = -1;
    EmojiStatus emoji_status;
    EmojiStatus last_sent_emoji_status;

    ProfilePhoto photo;

    vector<RestrictionReason> restriction_reasons;
    string inline_query_placeholder;
    int32 bot_info_version = -1;

    AccentColorId accent_color_id;
    CustomEmojiId background_custom_emoji_id;
    AccentColorId profile_accent_color_id;
    CustomEmojiId profile_background_custom_emoji_id;

    int32 was_online = 0;
    int32 local_was_online = 0;

    double max_active_story_id_next_reload_time = 0.0;
    StoryId max_active_story_id;
    StoryId max_read_story_id;

    string language_code;

    FlatHashSet<int64> photo_ids;

    static constexpr uint32 CACHE_VERSION = 4;
    uint32 cache_version = 0;

    bool is_min_access_hash = true;
    bool is_received = false;
    bool is_verified = false;
    bool is_premium = false;
    bool is_support = false;
    bool is_deleted = true;
    bool is_bot = true;
    bool can_join_groups = true;
    bool can_read_all_group_messages = true;
    bool can_be_edited_bot = false;
    bool is_inline_bot = false;
    bool need_location_bot = false;
    bool is_scam = false;
    bool is_fake = false;
    bool is_contact = false;
    bool is_mutual_contact = false;
    bool is_close_friend = false;
    bool need_apply_min_photo = false;
    bool can_be_added_to_attach_menu = false;
    bool attach_menu_enabled = false;
    bool stories_hidden = false;
    bool contact_require_premium = false;

    bool is_photo_inited = false;

    bool is_repaired = false;  // whether cached value is rechecked

    bool is_max_active_story_id_being_reloaded = false;

    bool is_name_changed = true;
    bool is_username_changed = true;
    bool is_photo_changed = true;
    bool is_accent_color_changed = true;
    bool is_phone_number_changed = true;
    bool is_emoji_status_changed = true;
    bool is_is_contact_changed = true;
    bool is_is_mutual_contact_changed = true;
    bool is_is_deleted_changed = true;
    bool is_is_premium_changed = true;
    bool is_stories_hidden_changed = true;
    bool is_full_info_changed = false;
    bool is_being_updated = false;
    bool is_changed = true;             // have new changes that need to be sent to the client and database
    bool need_save_to_database = true;  // have new changes that need only to be saved to the database
    bool is_status_changed = true;
    bool is_online_status_changed = true;  // whether online/offline has changed
    bool is_update_user_sent = false;

    bool is_saved = false;         // is current user version being saved/is saved to the database
    bool is_being_saved = false;   // is current user being saved to the database
    bool is_status_saved = false;  // is current user status being saved/is saved to the database

    bool is_received_from_server = false;  // true, if the user was received from the server and not the database

    uint64 log_event_id = 0;

    template <class StorerT>
    void store(StorerT &storer) const;

    template <class ParserT>
    void parse(ParserT &parser);
  };

  // do not forget to update drop_user_full and on_get_user_full
  struct UserFull {
    Photo photo;
    Photo fallback_photo;
    Photo personal_photo;

    string about;
    string private_forward_name;
    string description;
    Photo description_photo;
    FileId description_animation_file_id;
    vector<FileId> registered_file_ids;
    FileSourceId file_source_id;

    vector<PremiumGiftOption> premium_gift_options;

    unique_ptr<BotMenuButton> menu_button;
    vector<BotCommand> commands;
    AdministratorRights group_administrator_rights;
    AdministratorRights broadcast_administrator_rights;

    int32 common_chat_count = 0;

    bool is_blocked = false;
    bool is_blocked_for_stories = false;
    bool can_be_called = false;
    bool supports_video_calls = false;
    bool has_private_calls = false;
    bool can_pin_messages = true;
    bool need_phone_number_privacy_exception = false;
    bool wallpaper_overridden = false;
    bool voice_messages_forbidden = false;
    bool has_pinned_stories = false;
    bool read_dates_private = false;
    bool contact_require_premium = false;

    bool is_common_chat_count_changed = true;
    bool is_being_updated = false;
    bool is_changed = true;             // have new changes that need to be sent to the client and database
    bool need_send_update = true;       // have new changes that need only to be sent to the client
    bool need_save_to_database = true;  // have new changes that need only to be saved to the database
    bool is_update_user_full_sent = false;

    double expires_at = 0.0;

    bool is_expired() const {
      return expires_at < Time::now();
    }

    template <class StorerT>
    void store(StorerT &storer) const;

    template <class ParserT>
    void parse(ParserT &parser);
  };

  struct Chat {
    string title;
    DialogPhoto photo;
    int32 participant_count = 0;
    int32 date = 0;
    int32 version = -1;
    int32 default_permissions_version = -1;
    int32 pinned_message_version = -1;
    ChannelId migrated_to_channel_id;

    DialogParticipantStatus status = DialogParticipantStatus::Banned(0);
    RestrictedRights default_permissions{false, false, false, false, false, false, false, false, false,
                                         false, false, false, false, false, false, false, false, ChannelType::Unknown};

    static constexpr uint32 CACHE_VERSION = 4;
    uint32 cache_version = 0;

    bool is_active = false;
    bool noforwards = false;

    bool is_title_changed = true;
    bool is_photo_changed = true;
    bool is_default_permissions_changed = true;
    bool is_status_changed = true;
    bool is_is_active_changed = true;
    bool is_noforwards_changed = true;
    bool is_being_updated = false;
    bool is_changed = true;             // have new changes that need to be sent to the client and database
    bool need_save_to_database = true;  // have new changes that need only to be saved to the database
    bool is_update_basic_group_sent = false;

    bool is_repaired = false;  // whether cached value is rechecked

    bool is_saved = false;        // is current chat version being saved/is saved to the database
    bool is_being_saved = false;  // is current chat being saved to the database

    bool is_received_from_server = false;  // true, if the chat was received from the server and not the database

    uint64 log_event_id = 0;

    template <class StorerT>
    void store(StorerT &storer) const;

    template <class ParserT>
    void parse(ParserT &parser);
  };

  // do not forget to update drop_chat_full and on_get_chat_full
  struct ChatFull {
    int32 version = -1;
    UserId creator_user_id;
    vector<DialogParticipant> participants;

    Photo photo;
    vector<FileId> registered_photo_file_ids;
    FileSourceId file_source_id;

    string description;

    DialogInviteLink invite_link;

    vector<BotCommands> bot_commands;

    bool can_set_username = false;

    bool is_being_updated = false;
    bool is_changed = true;             // have new changes that need to be sent to the client and database
    bool need_send_update = true;       // have new changes that need only to be sent to the client
    bool need_save_to_database = true;  // have new changes that need only to be saved to the database
    bool is_update_chat_full_sent = false;

    template <class StorerT>
    void store(StorerT &storer) const;

    template <class ParserT>
    void parse(ParserT &parser);
  };

  struct Channel {
    int64 access_hash = 0;
    string title;
    DialogPhoto photo;
    EmojiStatus emoji_status;
    EmojiStatus last_sent_emoji_status;
    AccentColorId accent_color_id;
    CustomEmojiId background_custom_emoji_id;
    AccentColorId profile_accent_color_id;
    CustomEmojiId profile_background_custom_emoji_id;
    Usernames usernames;
    vector<RestrictionReason> restriction_reasons;
    DialogParticipantStatus status = DialogParticipantStatus::Banned(0);
    RestrictedRights default_permissions{false, false, false, false, false, false, false, false, false,
                                         false, false, false, false, false, false, false, false, ChannelType::Unknown};
    int32 date = 0;
    int32 participant_count = 0;
    int32 boost_level = 0;

    double max_active_story_id_next_reload_time = 0.0;
    StoryId max_active_story_id;
    StoryId max_read_story_id;

    static constexpr uint32 CACHE_VERSION = 10;
    uint32 cache_version = 0;

    bool has_linked_channel = false;
    bool has_location = false;
    bool sign_messages = false;
    bool is_slow_mode_enabled = false;
    bool noforwards = false;
    bool can_be_deleted = false;
    bool join_to_send = false;
    bool join_request = false;
    bool stories_hidden = false;

    bool is_megagroup = false;
    bool is_gigagroup = false;
    bool is_forum = false;
    bool is_verified = false;
    bool is_scam = false;
    bool is_fake = false;

    bool is_max_active_story_id_being_reloaded = false;

    bool is_title_changed = true;
    bool is_username_changed = true;
    bool is_photo_changed = true;
    bool is_emoji_status_changed = true;
    bool is_accent_color_changed = true;
    bool is_default_permissions_changed = true;
    bool is_status_changed = true;
    bool is_stories_hidden_changed = true;
    bool is_has_location_changed = true;
    bool is_noforwards_changed = true;
    bool is_creator_changed = true;
    bool had_read_access = true;
    bool was_member = false;
    bool is_being_updated = false;
    bool is_changed = true;             // have new changes that need to be sent to the client and database
    bool need_save_to_database = true;  // have new changes that need only to be saved to the database
    bool is_update_supergroup_sent = false;

    bool is_repaired = false;  // whether cached value is rechecked

    bool is_saved = false;        // is current channel version being saved/is saved to the database
    bool is_being_saved = false;  // is current channel being saved to the database

    bool is_received_from_server = false;  // true, if the channel was received from the server and not the database

    uint64 log_event_id = 0;

    template <class StorerT>
    void store(StorerT &storer) const;

    template <class ParserT>
    void parse(ParserT &parser);
  };

  // do not forget to update invalidate_channel_full and on_get_chat_full
  struct ChannelFull {
    Photo photo;
    vector<FileId> registered_photo_file_ids;
    FileSourceId file_source_id;

    string description;
    int32 participant_count = 0;
    int32 administrator_count = 0;
    int32 restricted_count = 0;
    int32 banned_count = 0;
    int32 boost_count = 0;
    int32 unrestrict_boost_count = 0;

    DialogInviteLink invite_link;

    vector<BotCommands> bot_commands;

    uint32 speculative_version = 1;
    uint32 repair_request_version = 0;

    StickerSetId sticker_set_id;
    StickerSetId emoji_sticker_set_id;

    ChannelId linked_channel_id;

    DialogLocation location;

    DcId stats_dc_id;

    int32 slow_mode_delay = 0;
    int32 slow_mode_next_send_date = 0;

    MessageId migrated_from_max_message_id;
    ChatId migrated_from_chat_id;

    vector<UserId> bot_user_ids;

    bool can_get_participants = false;
    bool has_hidden_participants = false;
    bool can_set_username = false;
    bool can_set_sticker_set = false;
    bool can_set_location = false;
    bool can_view_statistics = false;
    bool is_can_view_statistics_inited = false;
    bool is_all_history_available = true;
    bool has_aggressive_anti_spam_enabled = false;
    bool can_be_deleted = false;
    bool has_pinned_stories = false;

    bool is_slow_mode_next_send_date_changed = true;
    bool is_being_updated = false;
    bool is_changed = true;             // have new changes that need to be sent to the client and database
    bool need_send_update = true;       // have new changes that need only to be sent to the client
    bool need_save_to_database = true;  // have new changes that need only to be saved to the database
    bool is_update_channel_full_sent = false;

    double expires_at = 0.0;

    bool is_expired() const {
      return expires_at < Time::now();
    }

    template <class StorerT>
    void store(StorerT &storer) const;

    template <class ParserT>
    void parse(ParserT &parser);
  };

  struct SecretChat {
    int64 access_hash = 0;
    UserId user_id;
    SecretChatState state = SecretChatState::Unknown;
    string key_hash;
    int32 ttl = 0;
    int32 date = 0;
    int32 layer = 0;
    FolderId initial_folder_id;

    bool is_outbound = false;

    bool is_ttl_changed = true;
    bool is_state_changed = true;
    bool is_being_updated = false;
    bool is_changed = true;             // have new changes that need to be sent to the client and database
    bool need_save_to_database = true;  // have new changes that need only to be saved to the database

    bool is_saved = false;        // is current secret chat version being saved/is saved to the database
    bool is_being_saved = false;  // is current secret chat being saved to the database

    uint64 log_event_id = 0;

    template <class StorerT>
    void store(StorerT &storer) const;

    template <class ParserT>
    void parse(ParserT &parser);
  };

  struct PendingGetPhotoRequest {
    int32 offset = 0;
    int32 limit = 0;
    int32 retry_count = 0;
    Promise<td_api::object_ptr<td_api::chatPhotos>> promise;
  };

  struct UserPhotos {
    vector<Photo> photos;
    int32 count = -1;
    int32 offset = -1;

    vector<PendingGetPhotoRequest> pending_requests;
  };

  struct DialogNearby {
    DialogId dialog_id;
    int32 distance;

    DialogNearby(DialogId dialog_id, int32 distance) : dialog_id(dialog_id), distance(distance) {
    }

    bool operator<(const DialogNearby &other) const {
      return distance < other.distance || (distance == other.distance && dialog_id.get() < other.dialog_id.get());
    }

    bool operator==(const DialogNearby &other) const {
      return distance == other.distance && dialog_id == other.dialog_id;
    }

    bool operator!=(const DialogNearby &other) const {
      return !(*this == other);
    }
  };

  struct RecommendedDialogs {
    int32 total_count_ = 0;
    vector<DialogId> dialog_ids_;
    double next_reload_time_ = 0.0;

    template <class StorerT>
    void store(StorerT &storer) const;

    template <class ParserT>
    void parse(ParserT &parser);
  };

  class UserLogEvent;
  class ChatLogEvent;
  class ChannelLogEvent;
  class SecretChatLogEvent;

  static constexpr int32 MAX_GET_PROFILE_PHOTOS = 100;        // server side limit
  static constexpr size_t MAX_NAME_LENGTH = 64;               // server side limit for first/last name
  static constexpr size_t MAX_TITLE_LENGTH = 128;             // server side limit for chat title
  static constexpr size_t MAX_DESCRIPTION_LENGTH = 255;       // server side limit for chat/channel description
  static constexpr int32 MAX_GET_CHANNEL_PARTICIPANTS = 200;  // server side limit

  static constexpr int32 MAX_ACTIVE_STORY_ID_RELOAD_TIME = 3600;      // some reasonable limit
  static constexpr int32 CHANNEL_RECOMMENDATIONS_CACHE_TIME = 86400;  // some reasonable limit

  // the True fields aren't set for manually created telegram_api::user objects, therefore the flags must be used
  static constexpr int32 USER_FLAG_HAS_ACCESS_HASH = 1 << 0;
  static constexpr int32 USER_FLAG_HAS_FIRST_NAME = 1 << 1;
  static constexpr int32 USER_FLAG_HAS_LAST_NAME = 1 << 2;
  static constexpr int32 USER_FLAG_HAS_USERNAME = 1 << 3;
  static constexpr int32 USER_FLAG_HAS_PHONE_NUMBER = 1 << 4;
  static constexpr int32 USER_FLAG_HAS_PHOTO = 1 << 5;
  static constexpr int32 USER_FLAG_HAS_STATUS = 1 << 6;
  static constexpr int32 USER_FLAG_HAS_BOT_INFO_VERSION = 1 << 14;
  static constexpr int32 USER_FLAG_IS_ME = 1 << 10;
  static constexpr int32 USER_FLAG_IS_CONTACT = 1 << 11;
  static constexpr int32 USER_FLAG_IS_MUTUAL_CONTACT = 1 << 12;
  static constexpr int32 USER_FLAG_IS_DELETED = 1 << 13;
  static constexpr int32 USER_FLAG_IS_BOT = 1 << 14;
  static constexpr int32 USER_FLAG_IS_BOT_WITH_PRIVACY_DISABLED = 1 << 15;
  static constexpr int32 USER_FLAG_IS_PRIVATE_BOT = 1 << 16;
  static constexpr int32 USER_FLAG_IS_VERIFIED = 1 << 17;
  static constexpr int32 USER_FLAG_IS_RESTRICTED = 1 << 18;
  static constexpr int32 USER_FLAG_IS_INLINE_BOT = 1 << 19;
  static constexpr int32 USER_FLAG_IS_INACCESSIBLE = 1 << 20;
  static constexpr int32 USER_FLAG_NEED_LOCATION_BOT = 1 << 21;
  static constexpr int32 USER_FLAG_HAS_LANGUAGE_CODE = 1 << 22;
  static constexpr int32 USER_FLAG_IS_SUPPORT = 1 << 23;
  static constexpr int32 USER_FLAG_IS_SCAM = 1 << 24;
  static constexpr int32 USER_FLAG_NEED_APPLY_MIN_PHOTO = 1 << 25;
  static constexpr int32 USER_FLAG_IS_FAKE = 1 << 26;
  static constexpr int32 USER_FLAG_IS_ATTACH_MENU_BOT = 1 << 27;
  static constexpr int32 USER_FLAG_IS_PREMIUM = 1 << 28;
  static constexpr int32 USER_FLAG_ATTACH_MENU_ENABLED = 1 << 29;
  static constexpr int32 USER_FLAG_HAS_EMOJI_STATUS = 1 << 30;
  static constexpr int32 USER_FLAG_HAS_USERNAMES = 1 << 0;
  static constexpr int32 USER_FLAG_CAN_BE_EDITED_BOT = 1 << 1;
  static constexpr int32 USER_FLAG_IS_CLOSE_FRIEND = 1 << 2;

  static constexpr int32 USER_FULL_FLAG_IS_BLOCKED = 1 << 0;
  static constexpr int32 USER_FULL_FLAG_HAS_ABOUT = 1 << 1;
  static constexpr int32 USER_FULL_FLAG_HAS_PHOTO = 1 << 2;
  static constexpr int32 USER_FULL_FLAG_HAS_BOT_INFO = 1 << 3;
  static constexpr int32 USER_FULL_FLAG_HAS_PINNED_MESSAGE = 1 << 6;
  static constexpr int32 USER_FULL_FLAG_CAN_PIN_MESSAGE = 1 << 7;
  static constexpr int32 USER_FULL_FLAG_HAS_FOLDER_ID = 1 << 11;
  static constexpr int32 USER_FULL_FLAG_HAS_SCHEDULED_MESSAGES = 1 << 12;
  static constexpr int32 USER_FULL_FLAG_HAS_MESSAGE_TTL = 1 << 14;
  static constexpr int32 USER_FULL_FLAG_HAS_PRIVATE_FORWARD_NAME = 1 << 16;
  static constexpr int32 USER_FULL_FLAG_HAS_GROUP_ADMINISTRATOR_RIGHTS = 1 << 17;
  static constexpr int32 USER_FULL_FLAG_HAS_BROADCAST_ADMINISTRATOR_RIGHTS = 1 << 18;
  static constexpr int32 USER_FULL_FLAG_HAS_VOICE_MESSAGES_FORBIDDEN = 1 << 20;
  static constexpr int32 USER_FULL_FLAG_HAS_PERSONAL_PHOTO = 1 << 21;
  static constexpr int32 USER_FULL_FLAG_HAS_FALLBACK_PHOTO = 1 << 22;

  static constexpr int32 CHAT_FLAG_USER_IS_CREATOR = 1 << 0;
  static constexpr int32 CHAT_FLAG_USER_HAS_LEFT = 1 << 2;
  // static constexpr int32 CHAT_FLAG_ADMINISTRATORS_ENABLED = 1 << 3;
  // static constexpr int32 CHAT_FLAG_IS_ADMINISTRATOR = 1 << 4;
  static constexpr int32 CHAT_FLAG_IS_DEACTIVATED = 1 << 5;
  static constexpr int32 CHAT_FLAG_WAS_MIGRATED = 1 << 6;
  static constexpr int32 CHAT_FLAG_HAS_ACTIVE_GROUP_CALL = 1 << 23;
  static constexpr int32 CHAT_FLAG_IS_GROUP_CALL_NON_EMPTY = 1 << 24;
  static constexpr int32 CHAT_FLAG_NOFORWARDS = 1 << 25;

  static constexpr int32 CHAT_FULL_FLAG_HAS_PINNED_MESSAGE = 1 << 6;
  static constexpr int32 CHAT_FULL_FLAG_HAS_SCHEDULED_MESSAGES = 1 << 8;
  static constexpr int32 CHAT_FULL_FLAG_HAS_FOLDER_ID = 1 << 11;
  static constexpr int32 CHAT_FULL_FLAG_HAS_ACTIVE_GROUP_CALL = 1 << 12;
  static constexpr int32 CHAT_FULL_FLAG_HAS_MESSAGE_TTL = 1 << 14;
  static constexpr int32 CHAT_FULL_FLAG_HAS_PENDING_REQUEST_COUNT = 1 << 17;
  static constexpr int32 CHAT_FULL_FLAG_HAS_AVAILABLE_REACTIONS = 1 << 18;

  static constexpr int32 CHANNEL_FLAG_USER_IS_CREATOR = 1 << 0;
  static constexpr int32 CHANNEL_FLAG_USER_HAS_LEFT = 1 << 2;
  static constexpr int32 CHANNEL_FLAG_IS_BROADCAST = 1 << 5;
  static constexpr int32 CHANNEL_FLAG_HAS_USERNAME = 1 << 6;
  static constexpr int32 CHANNEL_FLAG_IS_VERIFIED = 1 << 7;
  static constexpr int32 CHANNEL_FLAG_IS_MEGAGROUP = 1 << 8;
  static constexpr int32 CHANNEL_FLAG_IS_RESTRICTED = 1 << 9;
  // static constexpr int32 CHANNEL_FLAG_ANYONE_CAN_INVITE = 1 << 10;
  static constexpr int32 CHANNEL_FLAG_SIGN_MESSAGES = 1 << 11;
  static constexpr int32 CHANNEL_FLAG_IS_MIN = 1 << 12;
  static constexpr int32 CHANNEL_FLAG_HAS_ACCESS_HASH = 1 << 13;
  static constexpr int32 CHANNEL_FLAG_HAS_ADMIN_RIGHTS = 1 << 14;
  static constexpr int32 CHANNEL_FLAG_HAS_BANNED_RIGHTS = 1 << 15;
  static constexpr int32 CHANNEL_FLAG_HAS_UNBAN_DATE = 1 << 16;
  static constexpr int32 CHANNEL_FLAG_HAS_PARTICIPANT_COUNT = 1 << 17;
  static constexpr int32 CHANNEL_FLAG_IS_SCAM = 1 << 19;
  static constexpr int32 CHANNEL_FLAG_HAS_LINKED_CHAT = 1 << 20;
  static constexpr int32 CHANNEL_FLAG_HAS_LOCATION = 1 << 21;
  static constexpr int32 CHANNEL_FLAG_IS_SLOW_MODE_ENABLED = 1 << 22;
  static constexpr int32 CHANNEL_FLAG_HAS_ACTIVE_GROUP_CALL = 1 << 23;
  static constexpr int32 CHANNEL_FLAG_IS_GROUP_CALL_NON_EMPTY = 1 << 24;
  static constexpr int32 CHANNEL_FLAG_IS_FAKE = 1 << 25;
  static constexpr int32 CHANNEL_FLAG_IS_GIGAGROUP = 1 << 26;
  static constexpr int32 CHANNEL_FLAG_NOFORWARDS = 1 << 27;
  static constexpr int32 CHANNEL_FLAG_JOIN_TO_SEND = 1 << 28;
  static constexpr int32 CHANNEL_FLAG_JOIN_REQUEST = 1 << 29;
  static constexpr int32 CHANNEL_FLAG_IS_FORUM = 1 << 30;
  static constexpr int32 CHANNEL_FLAG_HAS_USERNAMES = 1 << 0;

  static constexpr int32 CHANNEL_FULL_FLAG_HAS_PARTICIPANT_COUNT = 1 << 0;
  static constexpr int32 CHANNEL_FULL_FLAG_HAS_ADMINISTRATOR_COUNT = 1 << 1;
  static constexpr int32 CHANNEL_FULL_FLAG_HAS_BANNED_COUNT = 1 << 2;
  static constexpr int32 CHANNEL_FULL_FLAG_CAN_GET_PARTICIPANTS = 1 << 3;
  static constexpr int32 CHANNEL_FULL_FLAG_MIGRATED_FROM = 1 << 4;
  static constexpr int32 CHANNEL_FULL_FLAG_HAS_PINNED_MESSAGE = 1 << 5;
  static constexpr int32 CHANNEL_FULL_FLAG_CAN_SET_USERNAME = 1 << 6;
  static constexpr int32 CHANNEL_FULL_FLAG_CAN_SET_STICKER_SET = 1 << 7;
  static constexpr int32 CHANNEL_FULL_FLAG_HAS_STICKER_SET = 1 << 8;
  static constexpr int32 CHANNEL_FULL_FLAG_HAS_AVAILABLE_MIN_MESSAGE_ID = 1 << 9;
  static constexpr int32 CHANNEL_FULL_FLAG_IS_ALL_HISTORY_HIDDEN = 1 << 10;
  static constexpr int32 CHANNEL_FULL_FLAG_HAS_FOLDER_ID = 1 << 11;
  static constexpr int32 CHANNEL_FULL_FLAG_HAS_STATISTICS_DC_ID = 1 << 12;
  static constexpr int32 CHANNEL_FULL_FLAG_HAS_ONLINE_MEMBER_COUNT = 1 << 13;
  static constexpr int32 CHANNEL_FULL_FLAG_HAS_LINKED_CHANNEL_ID = 1 << 14;
  static constexpr int32 CHANNEL_FULL_FLAG_HAS_LOCATION = 1 << 15;
  static constexpr int32 CHANNEL_FULL_FLAG_CAN_SET_LOCATION = 1 << 16;
  static constexpr int32 CHANNEL_FULL_FLAG_HAS_SLOW_MODE_DELAY = 1 << 17;
  static constexpr int32 CHANNEL_FULL_FLAG_HAS_SLOW_MODE_NEXT_SEND_DATE = 1 << 18;
  static constexpr int32 CHANNEL_FULL_FLAG_HAS_SCHEDULED_MESSAGES = 1 << 19;
  static constexpr int32 CHANNEL_FULL_FLAG_CAN_VIEW_STATISTICS = 1 << 20;
  static constexpr int32 CHANNEL_FULL_FLAG_HAS_ACTIVE_GROUP_CALL = 1 << 21;
  static constexpr int32 CHANNEL_FULL_FLAG_IS_BLOCKED = 1 << 22;
  static constexpr int32 CHANNEL_FULL_FLAG_HAS_EXPORTED_INVITE = 1 << 23;
  static constexpr int32 CHANNEL_FULL_FLAG_HAS_MESSAGE_TTL = 1 << 24;
  static constexpr int32 CHANNEL_FULL_FLAG_HAS_PENDING_REQUEST_COUNT = 1 << 28;
  static constexpr int32 CHANNEL_FULL_FLAG_HAS_DEFAULT_SEND_AS = 1 << 29;
  static constexpr int32 CHANNEL_FULL_FLAG_HAS_AVAILABLE_REACTIONS = 1 << 30;
  static constexpr int32 CHANNEL_FULL_FLAG2_HAS_ANTISPAM = 1 << 1;
  static constexpr int32 CHANNEL_FULL_FLAG2_ARE_PARTICIPANTS_HIDDEN = 1 << 2;

  static constexpr int32 USER_FULL_EXPIRE_TIME = 60;
  static constexpr int32 CHANNEL_FULL_EXPIRE_TIME = 60;

  static constexpr int32 ACCOUNT_UPDATE_FIRST_NAME = 1 << 0;
  static constexpr int32 ACCOUNT_UPDATE_LAST_NAME = 1 << 1;
  static constexpr int32 ACCOUNT_UPDATE_ABOUT = 1 << 2;

  bool have_input_peer_user(const User *u, UserId user_id, AccessRights access_rights) const;
  static bool have_input_peer_chat(const Chat *c, AccessRights access_rights);
  bool have_input_peer_channel(const Channel *c, ChannelId channel_id, AccessRights access_rights,
                               bool from_linked = false) const;
  static bool have_input_encrypted_peer(const SecretChat *secret_chat, AccessRights access_rights);

  tl_object_ptr<telegram_api::InputPeer> get_simple_input_peer(DialogId dialog_id) const;

  const User *get_user(UserId user_id) const;
  User *get_user(UserId user_id);
  User *get_user_force(UserId user_id, const char *source);
  User *get_user_force_impl(UserId user_id, const char *source);

  User *add_user(UserId user_id);

  const UserFull *get_user_full(UserId user_id) const;
  UserFull *get_user_full(UserId user_id);
  UserFull *get_user_full_force(UserId user_id, const char *source);

  UserFull *add_user_full(UserId user_id);

  void send_get_user_full_query(UserId user_id, tl_object_ptr<telegram_api::InputUser> &&input_user,
                                Promise<Unit> &&promise, const char *source);

  const Chat *get_chat(ChatId chat_id) const;
  Chat *get_chat(ChatId chat_id);
  Chat *get_chat_force(ChatId chat_id, const char *source);

  Chat *add_chat(ChatId chat_id);

  const ChatFull *get_chat_full(ChatId chat_id) const;
  ChatFull *get_chat_full(ChatId chat_id);
  ChatFull *get_chat_full_force(ChatId chat_id, const char *source);

  ChatFull *add_chat_full(ChatId chat_id);

  void send_get_chat_full_query(ChatId chat_id, Promise<Unit> &&promise, const char *source);

  void on_migrate_chat_to_megagroup(ChatId chat_id, Promise<td_api::object_ptr<td_api::chat>> &&promise);

  const Channel *get_channel(ChannelId channel_id) const;
  Channel *get_channel(ChannelId channel_id);
  Channel *get_channel_force(ChannelId channel_id, const char *source);

  Channel *add_channel(ChannelId channel_id, const char *source);

  const ChannelFull *get_channel_full(ChannelId channel_id) const;
  const ChannelFull *get_channel_full_const(ChannelId channel_id) const;
  ChannelFull *get_channel_full(ChannelId channel_id, bool only_local, const char *source);
  ChannelFull *get_channel_full_force(ChannelId channel_id, bool only_local, const char *source);

  ChannelFull *add_channel_full(ChannelId channel_id);

  void send_get_channel_full_query(ChannelFull *channel_full, ChannelId channel_id, Promise<Unit> &&promise,
                                   const char *source);

  void on_create_new_secret_chat(SecretChatId secret_chat_id, Promise<td_api::object_ptr<td_api::chat>> &&promise);

  const SecretChat *get_secret_chat(SecretChatId secret_chat_id) const;
  SecretChat *get_secret_chat(SecretChatId secret_chat_id);
  SecretChat *get_secret_chat_force(SecretChatId secret_chat_id, const char *source);

  SecretChat *add_secret_chat(SecretChatId secret_chat_id);

  string get_user_search_text(UserId user_id) const;
  static string get_user_search_text(const User *u);

  static DialogParticipantStatus get_chat_status(const Chat *c);
  DialogParticipantStatus get_chat_permissions(const Chat *c) const;

  static ChannelType get_channel_type(const Channel *c);
  static DialogParticipantStatus get_channel_status(const Channel *c);
  DialogParticipantStatus get_channel_permissions(ChannelId channel_id, const Channel *c) const;
  static bool get_channel_sign_messages(const Channel *c);
  static bool get_channel_has_linked_channel(const Channel *c);
  static bool get_channel_can_be_deleted(const Channel *c);
  static bool get_channel_join_to_send(const Channel *c);
  static bool get_channel_join_request(const Channel *c);

  string get_channel_search_text(ChannelId channel_id) const;
  static string get_channel_search_text(const Channel *c);

  void set_my_id(UserId my_id);

  void on_set_emoji_status(EmojiStatus emoji_status, Promise<Unit> &&promise);

  void on_update_user_name(User *u, UserId user_id, string &&first_name, string &&last_name);
  void on_update_user_usernames(User *u, UserId user_id, Usernames &&usernames);
  void on_update_user_phone_number(User *u, UserId user_id, string &&phone_number);
  void on_update_user_photo(User *u, UserId user_id, tl_object_ptr<telegram_api::UserProfilePhoto> &&photo,
                            const char *source);
  void on_update_user_accent_color_id(User *u, UserId user_id, AccentColorId accent_color_id);
  void on_update_user_background_custom_emoji_id(User *u, UserId user_id, CustomEmojiId background_custom_emoji_id);
  void on_update_user_profile_accent_color_id(User *u, UserId user_id, AccentColorId accent_color_id);
  void on_update_user_profile_background_custom_emoji_id(User *u, UserId user_id,
                                                         CustomEmojiId background_custom_emoji_id);
  void on_update_user_emoji_status(User *u, UserId user_id, EmojiStatus emoji_status);
  void on_update_user_story_ids_impl(User *u, UserId user_id, StoryId max_active_story_id, StoryId max_read_story_id);
  void on_update_user_max_read_story_id(User *u, UserId user_id, StoryId max_read_story_id);
  void on_update_user_stories_hidden(User *u, UserId user_id, bool stories_hidden);
  void on_update_user_is_contact(User *u, UserId user_id, bool is_contact, bool is_mutual_contact,
                                 bool is_close_friend);
  void on_update_user_online(User *u, UserId user_id, tl_object_ptr<telegram_api::UserStatus> &&status);
  void on_update_user_local_was_online(User *u, UserId user_id, int32 local_was_online);

  void do_update_user_photo(User *u, UserId user_id, tl_object_ptr<telegram_api::UserProfilePhoto> &&photo,
                            const char *source);
  void do_update_user_photo(User *u, UserId user_id, ProfilePhoto &&new_photo, bool invalidate_photo_cache,
                            const char *source);
  void apply_pending_user_photo(User *u, UserId user_id);

  void set_profile_photo_impl(UserId user_id, const td_api::object_ptr<td_api::InputChatPhoto> &input_photo,
                              bool is_fallback, bool only_suggest, Promise<Unit> &&promise);

  void upload_profile_photo(UserId user_id, FileId file_id, bool is_fallback, bool only_suggest, bool is_animation,
                            double main_frame_timestamp, Promise<Unit> &&promise, int reupload_count = 0,
                            vector<int> bad_parts = {});

  void on_upload_profile_photo(FileId file_id, tl_object_ptr<telegram_api::InputFile> input_file);
  void on_upload_profile_photo_error(FileId file_id, Status status);

  void register_user_photo(User *u, UserId user_id, const Photo &photo);

  static void on_update_user_full_is_blocked(UserFull *user_full, UserId user_id, bool is_blocked,
                                             bool is_blocked_for_stories);
  static void on_update_user_full_common_chat_count(UserFull *user_full, UserId user_id, int32 common_chat_count);
  static void on_update_user_full_commands(UserFull *user_full, UserId user_id,
                                           vector<tl_object_ptr<telegram_api::botCommand>> &&bot_commands);
  static void on_update_user_full_menu_button(UserFull *user_full, UserId user_id,
                                              tl_object_ptr<telegram_api::BotMenuButton> &&bot_menu_button);
  void on_update_user_full_need_phone_number_privacy_exception(UserFull *user_full, UserId user_id,
                                                               bool need_phone_number_privacy_exception) const;
  void on_update_user_full_wallpaper_overridden(UserFull *user_full, UserId user_id, bool wallpaper_overridden) const;

  UserPhotos *add_user_photos(UserId user_id);
  void send_get_user_photos_query(UserId user_id, const UserPhotos *user_photos);
  void on_get_user_profile_photos(UserId user_id, Result<Unit> &&result);
  int64 get_user_full_profile_photo_id(const UserFull *user_full);
  void add_set_profile_photo_to_cache(UserId user_id, Photo &&photo, bool is_fallback);
  bool delete_my_profile_photo_from_cache(int64 profile_photo_id);
  void drop_user_full_photos(UserFull *user_full, UserId user_id, int64 expected_photo_id, const char *source);
  void drop_user_photos(UserId user_id, bool is_empty, const char *source);
  void drop_user_full(UserId user_id);

  void on_update_chat_status(Chat *c, ChatId chat_id, DialogParticipantStatus status);
  static void on_update_chat_default_permissions(Chat *c, ChatId chat_id, RestrictedRights default_permissions,
                                                 int32 version);
  void on_update_chat_participant_count(Chat *c, ChatId chat_id, int32 participant_count, int32 version,
                                        const string &debug_str);
  void on_update_chat_photo(Chat *c, ChatId chat_id, tl_object_ptr<telegram_api::ChatPhoto> &&chat_photo_ptr);
  void on_update_chat_photo(Chat *c, ChatId chat_id, DialogPhoto &&photo, bool invalidate_photo_cache);
  static void on_update_chat_title(Chat *c, ChatId chat_id, string &&title);
  static void on_update_chat_active(Chat *c, ChatId chat_id, bool is_active);
  static void on_update_chat_migrated_to_channel_id(Chat *c, ChatId chat_id, ChannelId migrated_to_channel_id);
  static void on_update_chat_noforwards(Chat *c, ChatId chat_id, bool noforwards);

  void on_update_chat_full_photo(ChatFull *chat_full, ChatId chat_id, Photo photo);
  bool on_update_chat_full_participants_short(ChatFull *chat_full, ChatId chat_id, int32 version);
  void on_update_chat_full_participants(ChatFull *chat_full, ChatId chat_id, vector<DialogParticipant> participants,
                                        int32 version, bool from_update);
  void on_update_chat_full_invite_link(ChatFull *chat_full,
                                       tl_object_ptr<telegram_api::ExportedChatInvite> &&invite_link);

  void on_update_channel_photo(Channel *c, ChannelId channel_id,
                               tl_object_ptr<telegram_api::ChatPhoto> &&chat_photo_ptr);
  void on_update_channel_photo(Channel *c, ChannelId channel_id, DialogPhoto &&photo, bool invalidate_photo_cache);
  void on_update_channel_emoji_status(Channel *c, ChannelId channel_id, EmojiStatus emoji_status);
  void on_update_channel_accent_color_id(Channel *c, ChannelId channel_id, AccentColorId accent_color_id);
  void on_update_channel_background_custom_emoji_id(Channel *c, ChannelId channel_id,
                                                    CustomEmojiId background_custom_emoji_id);
  void on_update_channel_profile_accent_color_id(Channel *c, ChannelId channel_id,
                                                 AccentColorId profile_accent_color_id);
  void on_update_channel_profile_background_custom_emoji_id(Channel *c, ChannelId channel_id,
                                                            CustomEmojiId profile_background_custom_emoji_id);
  static void on_update_channel_title(Channel *c, ChannelId channel_id, string &&title);
  void on_update_channel_usernames(Channel *c, ChannelId channel_id, Usernames &&usernames);
  void on_update_channel_status(Channel *c, ChannelId channel_id, DialogParticipantStatus &&status);
  static void on_update_channel_default_permissions(Channel *c, ChannelId channel_id,
                                                    RestrictedRights default_permissions);
  static void on_update_channel_has_location(Channel *c, ChannelId channel_id, bool has_location);
  static void on_update_channel_noforwards(Channel *c, ChannelId channel_id, bool noforwards);
  void on_update_channel_stories_hidden(Channel *c, ChannelId channel_id, bool stories_hidden);
  void on_update_channel_story_ids_impl(Channel *c, ChannelId channel_id, StoryId max_active_story_id,
                                        StoryId max_read_story_id);
  void on_update_channel_max_read_story_id(Channel *c, ChannelId channel_id, StoryId max_read_story_id);

  void on_update_channel_bot_user_ids(ChannelId channel_id, vector<UserId> &&bot_user_ids);

  void on_update_channel_full_photo(ChannelFull *channel_full, ChannelId channel_id, Photo photo);
  void on_update_channel_full_invite_link(ChannelFull *channel_full,
                                          tl_object_ptr<telegram_api::ExportedChatInvite> &&invite_link);
  void on_update_channel_full_linked_channel_id(ChannelFull *channel_full, ChannelId channel_id,
                                                ChannelId linked_channel_id);
  void on_update_channel_full_location(ChannelFull *channel_full, ChannelId channel_id, const DialogLocation &location);
  void on_update_channel_full_slow_mode_delay(ChannelFull *channel_full, ChannelId channel_id, int32 slow_mode_delay,
                                              int32 slow_mode_next_send_date);
  static void on_update_channel_full_slow_mode_next_send_date(ChannelFull *channel_full,
                                                              int32 slow_mode_next_send_date);
  static void on_update_channel_full_bot_user_ids(ChannelFull *channel_full, ChannelId channel_id,
                                                  vector<UserId> &&bot_user_ids);

  void toggle_username_is_active_impl(string &&username, bool is_active, Promise<Unit> &&promise);

  void reorder_usernames_impl(vector<string> &&usernames, Promise<Unit> &&promise);

  void on_channel_status_changed(Channel *c, ChannelId channel_id, const DialogParticipantStatus &old_status,
                                 const DialogParticipantStatus &new_status);
  void on_channel_usernames_changed(const Channel *c, ChannelId channel_id, const Usernames &old_usernames,
                                    const Usernames &new_usernames);

  void remove_linked_channel_id(ChannelId channel_id);
  ChannelId get_linked_channel_id(ChannelId channel_id) const;

  static bool speculative_add_count(int32 &count, int32 delta_count, int32 min_count = 0);

  void speculative_add_channel_participant_count(ChannelId channel_id, int32 delta_participant_count, bool by_me);

  void drop_chat_full(ChatId chat_id);

  void do_invalidate_channel_full(ChannelFull *channel_full, ChannelId channel_id, bool need_drop_slow_mode_delay);

  void update_user_online_member_count(UserId user_id);
  void update_chat_online_member_count(const ChatFull *chat_full, ChatId chat_id, bool is_from_server);
  void update_channel_online_member_count(ChannelId channel_id, bool is_from_server);
  void update_dialog_online_member_count(const vector<DialogParticipant> &participants, DialogId dialog_id,
                                         bool is_from_server);

  void on_get_chat_empty(telegram_api::chatEmpty &chat, const char *source);
  void on_get_chat(telegram_api::chat &chat, const char *source);
  void on_get_chat_forbidden(telegram_api::chatForbidden &chat, const char *source);
  void on_get_channel(telegram_api::channel &channel, const char *source);
  void on_get_channel_forbidden(telegram_api::channelForbidden &channel, const char *source);

  void save_user(User *u, UserId user_id, bool from_binlog);
  static string get_user_database_key(UserId user_id);
  static string get_user_database_value(const User *u);
  void save_user_to_database(User *u, UserId user_id);
  void save_user_to_database_impl(User *u, UserId user_id, string value);
  void on_save_user_to_database(UserId user_id, bool success);
  void load_user_from_database(User *u, UserId user_id, Promise<Unit> promise);
  void load_user_from_database_impl(UserId user_id, Promise<Unit> promise);
  void on_load_user_from_database(UserId user_id, string value, bool force);

  void save_chat(Chat *c, ChatId chat_id, bool from_binlog);
  static string get_chat_database_key(ChatId chat_id);
  static string get_chat_database_value(const Chat *c);
  void save_chat_to_database(Chat *c, ChatId chat_id);
  void save_chat_to_database_impl(Chat *c, ChatId chat_id, string value);
  void on_save_chat_to_database(ChatId chat_id, bool success);
  void load_chat_from_database(Chat *c, ChatId chat_id, Promise<Unit> promise);
  void load_chat_from_database_impl(ChatId chat_id, Promise<Unit> promise);
  void on_load_chat_from_database(ChatId chat_id, string value, bool force);

  void save_channel(Channel *c, ChannelId channel_id, bool from_binlog);
  static string get_channel_database_key(ChannelId channel_id);
  static string get_channel_database_value(const Channel *c);
  void save_channel_to_database(Channel *c, ChannelId channel_id);
  void save_channel_to_database_impl(Channel *c, ChannelId channel_id, string value);
  void on_save_channel_to_database(ChannelId channel_id, bool success);
  void load_channel_from_database(Channel *c, ChannelId channel_id, Promise<Unit> promise);
  void load_channel_from_database_impl(ChannelId channel_id, Promise<Unit> promise);
  void on_load_channel_from_database(ChannelId channel_id, string value, bool force);

  void save_secret_chat(SecretChat *c, SecretChatId secret_chat_id, bool from_binlog);
  static string get_secret_chat_database_key(SecretChatId secret_chat_id);
  static string get_secret_chat_database_value(const SecretChat *c);
  void save_secret_chat_to_database(SecretChat *c, SecretChatId secret_chat_id);
  void save_secret_chat_to_database_impl(SecretChat *c, SecretChatId secret_chat_id, string value);
  void on_save_secret_chat_to_database(SecretChatId secret_chat_id, bool success);
  void load_secret_chat_from_database(SecretChat *c, SecretChatId secret_chat_id, Promise<Unit> promise);
  void load_secret_chat_from_database_impl(SecretChatId secret_chat_id, Promise<Unit> promise);
  void on_load_secret_chat_from_database(SecretChatId secret_chat_id, string value, bool force);

  static void save_user_full(const UserFull *user_full, UserId user_id);
  static string get_user_full_database_key(UserId user_id);
  static string get_user_full_database_value(const UserFull *user_full);
  void on_load_user_full_from_database(UserId user_id, string value);

  static void save_chat_full(const ChatFull *chat_full, ChatId chat_id);
  static string get_chat_full_database_key(ChatId chat_id);
  static string get_chat_full_database_value(const ChatFull *chat_full);
  void on_load_chat_full_from_database(ChatId chat_id, string value);

  static void save_channel_full(const ChannelFull *channel_full, ChannelId channel_id);
  static string get_channel_full_database_key(ChannelId channel_id);
  static string get_channel_full_database_value(const ChannelFull *channel_full);
  void on_load_channel_full_from_database(ChannelId channel_id, string value, const char *source);

  void update_user(User *u, UserId user_id, bool from_binlog = false, bool from_database = false);
  void update_chat(Chat *c, ChatId chat_id, bool from_binlog = false, bool from_database = false);
  void update_channel(Channel *c, ChannelId channel_id, bool from_binlog = false, bool from_database = false);
  void update_secret_chat(SecretChat *c, SecretChatId secret_chat_id, bool from_binlog = false,
                          bool from_database = false);

  void update_user_full(UserFull *user_full, UserId user_id, const char *source, bool from_database = false);
  void update_chat_full(ChatFull *chat_full, ChatId chat_id, const char *source, bool from_database = false);
  void update_channel_full(ChannelFull *channel_full, ChannelId channel_id, const char *source,
                           bool from_database = false);

  bool is_chat_full_outdated(const ChatFull *chat_full, const Chat *c, ChatId chat_id, bool only_participants) const;

  bool is_user_contact(const User *u, UserId user_id, bool is_mutual) const;

  static bool is_user_premium(const User *u);

  static bool is_user_deleted(const User *u);

  static bool is_user_support(const User *u);

  static bool is_user_bot(const User *u);

  int32 get_user_was_online(const User *u, UserId user_id, int32 unix_time) const;

  int64 get_contacts_hash();

  void update_contacts_hints(const User *u, UserId user_id, bool from_database);

  void save_next_contacts_sync_date();

  void save_contacts_to_database();

  void load_contacts(Promise<Unit> &&promise);

  void on_load_contacts_from_database(string value);

  void on_get_contacts_finished(size_t expected_contact_count);

  void do_import_contacts(vector<Contact> contacts, int64 random_id, Promise<Unit> &&promise);

  void on_import_contacts_finished(int64 random_id, vector<UserId> imported_contact_user_ids,
                                   vector<int32> unimported_contact_invites);

  void load_imported_contacts(Promise<Unit> &&promise);

  void on_load_imported_contacts_from_database(string value);

  void on_load_imported_contacts_finished();

  void on_clear_imported_contacts(vector<Contact> &&contacts, vector<size_t> contacts_unique_id,
                                  std::pair<vector<size_t>, vector<Contact>> &&to_add, Promise<Unit> &&promise);

  vector<td_api::object_ptr<td_api::chatNearby>> get_chats_nearby_object(
      const vector<DialogNearby> &dialogs_nearby) const;

  void send_update_users_nearby() const;

  void on_get_dialogs_nearby(Result<tl_object_ptr<telegram_api::Updates>> result,
                             Promise<td_api::object_ptr<td_api::chatsNearby>> &&promise);

  void try_send_set_location_visibility_query();

  void on_set_location_visibility_expire_date(int32 set_expire_date, int32 error_code);

  void set_location_visibility_expire_date(int32 expire_date);

  void on_get_is_location_visible(Result<tl_object_ptr<telegram_api::Updates>> &&result, Promise<Unit> &&promise);

  void update_is_location_visible();

  bool is_suitable_recommended_channel(DialogId dialog_id) const;

  bool is_suitable_recommended_channel(ChannelId channel_id) const;

  bool are_suitable_recommended_dialogs(const RecommendedDialogs &recommended_dialogs) const;

  static string get_channel_recommendations_database_key(ChannelId channel_id);

  void load_channel_recommendations(ChannelId channel_id, bool use_database, bool return_local,
                                    Promise<td_api::object_ptr<td_api::chats>> &&chats_promise,
                                    Promise<td_api::object_ptr<td_api::count>> &&count_promise);

  void fail_load_channel_recommendations_queries(ChannelId channel_id, Status &&error);

  void finish_load_channel_recommendations_queries(ChannelId channel_id, int32 total_count,
                                                   vector<DialogId> dialog_ids);

  void on_load_channel_recommendations_from_database(ChannelId channel_id, string value);

  void reload_channel_recommendations(ChannelId channel_id);

  void on_get_channel_recommendations(ChannelId channel_id,
                                      Result<std::pair<int32, vector<tl_object_ptr<telegram_api::Chat>>>> &&r_chats);

  static bool is_channel_public(const Channel *c);

  static bool is_suitable_created_public_channel(PublicDialogType type, const Channel *c);

  static void return_created_public_dialogs(Promise<td_api::object_ptr<td_api::chats>> &&promise,
                                            const vector<ChannelId> &channel_ids);

  void finish_get_created_public_dialogs(PublicDialogType type, Result<Unit> &&result);

  void update_created_public_channels(Channel *c, ChannelId channel_id);

  void save_created_public_channels(PublicDialogType type);

  void update_created_public_broadcasts();

  bool update_permanent_invite_link(DialogInviteLink &invite_link, DialogInviteLink new_invite_link);

  vector<BotCommands> get_bot_commands(vector<tl_object_ptr<telegram_api::botInfo>> &&bot_infos,
                                       const vector<DialogParticipant> *participants);

  const DialogParticipant *get_chat_participant(ChatId chat_id, UserId user_id) const;

  static const DialogParticipant *get_chat_full_participant(const ChatFull *chat_full, DialogId dialog_id);

  std::pair<int32, vector<DialogId>> search_among_dialogs(const vector<DialogId> &dialog_ids, const string &query,
                                                          int32 limit) const;

  DialogParticipants search_private_chat_participants(UserId my_user_id, UserId peer_user_id, const string &query,
                                                      int32 limit, DialogParticipantFilter filter) const;

  void finish_get_chat_participant(ChatId chat_id, UserId user_id, Promise<DialogParticipant> &&promise);

  void remove_dialog_suggested_action(SuggestedAction action);

  void on_dismiss_suggested_action(SuggestedAction action, Result<Unit> &&result);

  bool need_poll_user_active_stories(const User *u, UserId user_id) const;

  static bool get_user_has_unread_stories(const User *u);

  td_api::object_ptr<td_api::updateUser> get_update_user_object(UserId user_id, const User *u) const;

  td_api::object_ptr<td_api::updateUser> get_update_unknown_user_object(UserId user_id) const;

  td_api::object_ptr<td_api::UserStatus> get_user_status_object(UserId user_id, const User *u, int32 unix_time) const;

  tl_object_ptr<td_api::user> get_user_object(UserId user_id, const User *u) const;

  tl_object_ptr<td_api::userFullInfo> get_user_full_info_object(UserId user_id, const UserFull *user_full) const;

  td_api::object_ptr<td_api::updateBasicGroup> get_update_basic_group_object(ChatId chat_id, const Chat *c);

  static td_api::object_ptr<td_api::updateBasicGroup> get_update_unknown_basic_group_object(ChatId chat_id);

  tl_object_ptr<td_api::basicGroup> get_basic_group_object(ChatId chat_id, const Chat *c);

  tl_object_ptr<td_api::basicGroup> get_basic_group_object_const(ChatId chat_id, const Chat *c) const;

  tl_object_ptr<td_api::basicGroupFullInfo> get_basic_group_full_info_object(ChatId chat_id,
                                                                             const ChatFull *chat_full) const;

  bool need_poll_channel_active_stories(const Channel *c, ChannelId channel_id) const;

  static bool get_channel_has_unread_stories(const Channel *c);

  td_api::object_ptr<td_api::updateSupergroup> get_update_supergroup_object(ChannelId channel_id,
                                                                            const Channel *c) const;

  td_api::object_ptr<td_api::updateSupergroup> get_update_unknown_supergroup_object(ChannelId channel_id) const;

  static tl_object_ptr<td_api::supergroup> get_supergroup_object(ChannelId channel_id, const Channel *c);

  Status can_hide_chat_participants(ChatId chat_id) const;

  Status can_hide_channel_participants(ChannelId channel_id, const ChannelFull *channel_full) const;

  Status can_toggle_chat_aggressive_anti_spam(ChatId chat_id) const;

  Status can_toggle_channel_aggressive_anti_spam(ChannelId channel_id, const ChannelFull *channel_full) const;

  tl_object_ptr<td_api::supergroupFullInfo> get_supergroup_full_info_object(ChannelId channel_id,
                                                                            const ChannelFull *channel_full) const;

  static tl_object_ptr<td_api::SecretChatState> get_secret_chat_state_object(SecretChatState state);

  td_api::object_ptr<td_api::updateSecretChat> get_update_secret_chat_object(SecretChatId secret_chat_id,
                                                                             const SecretChat *secret_chat);

  static td_api::object_ptr<td_api::updateSecretChat> get_update_unknown_secret_chat_object(
      SecretChatId secret_chat_id);

  tl_object_ptr<td_api::secretChat> get_secret_chat_object(SecretChatId secret_chat_id, const SecretChat *secret_chat);

  tl_object_ptr<td_api::secretChat> get_secret_chat_object_const(SecretChatId secret_chat_id,
                                                                 const SecretChat *secret_chat) const;

  vector<DialogId> get_dialog_ids(vector<tl_object_ptr<telegram_api::Chat>> &&chats, const char *source);

  void on_create_inactive_channels(vector<ChannelId> &&channel_ids, Promise<Unit> &&promise);

  void update_dialogs_for_discussion(DialogId dialog_id, bool is_suitable);

  void send_edit_chat_admin_query(ChatId chat_id, UserId user_id, bool is_administrator, Promise<Unit> &&promise);

  void search_chat_participants(ChatId chat_id, const string &query, int32 limit, DialogParticipantFilter filter,
                                Promise<DialogParticipants> &&promise);

  void do_search_chat_participants(ChatId chat_id, const string &query, int32 limit, DialogParticipantFilter filter,
                                   Promise<DialogParticipants> &&promise);

  void on_get_channel_participants(ChannelId channel_id, ChannelParticipantFilter &&filter, int32 offset, int32 limit,
                                   string additional_query, int32 additional_limit,
                                   tl_object_ptr<telegram_api::channels_channelParticipants> &&channel_participants,
                                   Promise<DialogParticipants> &&promise);

  void transfer_channel_ownership(ChannelId channel_id, UserId user_id,
                                  tl_object_ptr<telegram_api::InputCheckPasswordSRP> input_check_password,
                                  Promise<Unit> &&promise);

  void get_channel_statistics_dc_id_impl(ChannelId channel_id, bool for_full_statistics, Promise<DcId> &&promise);

  void on_get_support_user(UserId user_id, Promise<td_api::object_ptr<td_api::user>> &&promise);

  static void on_user_online_timeout_callback(void *contacts_manager_ptr, int64 user_id_long);

  static void on_user_emoji_status_timeout_callback(void *contacts_manager_ptr, int64 user_id_long);

  static void on_channel_emoji_status_timeout_callback(void *contacts_manager_ptr, int64 channel_id_long);

  static void on_channel_unban_timeout_callback(void *contacts_manager_ptr, int64 channel_id_long);

  static void on_user_nearby_timeout_callback(void *contacts_manager_ptr, int64 user_id_long);

  static void on_slow_mode_delay_timeout_callback(void *contacts_manager_ptr, int64 channel_id_long);

  void on_user_online_timeout(UserId user_id);

  void on_user_emoji_status_timeout(UserId user_id);

  void on_channel_emoji_status_timeout(ChannelId channel_id);

  void on_channel_unban_timeout(ChannelId channel_id);

  void on_user_nearby_timeout(UserId user_id);

  void on_slow_mode_delay_timeout(ChannelId channel_id);

  void start_up() final;

  void tear_down() final;

  Td *td_;
  ActorShared<> parent_;
  UserId my_id_;
  UserId support_user_id_;
  int32 my_was_online_local_ = 0;

  WaitFreeHashMap<UserId, unique_ptr<User>, UserIdHash> users_;
  WaitFreeHashMap<UserId, unique_ptr<UserFull>, UserIdHash> users_full_;
  WaitFreeHashMap<UserId, unique_ptr<UserPhotos>, UserIdHash> user_photos_;
  mutable FlatHashSet<UserId, UserIdHash> unknown_users_;
  WaitFreeHashMap<UserId, tl_object_ptr<telegram_api::UserProfilePhoto>, UserIdHash> pending_user_photos_;
  struct UserIdPhotoIdHash {
    uint32 operator()(const std::pair<UserId, int64> &pair) const {
      return combine_hashes(UserIdHash()(pair.first), Hash<int64>()(pair.second));
    }
  };
  WaitFreeHashMap<std::pair<UserId, int64>, FileSourceId, UserIdPhotoIdHash> user_profile_photo_file_source_ids_;
  FlatHashMap<int64, FileId> my_photo_file_id_;
  WaitFreeHashMap<UserId, FileSourceId, UserIdHash> user_full_file_source_ids_;

  WaitFreeHashMap<ChatId, unique_ptr<Chat>, ChatIdHash> chats_;
  WaitFreeHashMap<ChatId, unique_ptr<ChatFull>, ChatIdHash> chats_full_;
  mutable FlatHashSet<ChatId, ChatIdHash> unknown_chats_;
  WaitFreeHashMap<ChatId, FileSourceId, ChatIdHash> chat_full_file_source_ids_;

  WaitFreeHashMap<ChannelId, unique_ptr<MinChannel>, ChannelIdHash> min_channels_;
  WaitFreeHashMap<ChannelId, unique_ptr<Channel>, ChannelIdHash> channels_;
  WaitFreeHashMap<ChannelId, unique_ptr<ChannelFull>, ChannelIdHash> channels_full_;
  mutable FlatHashSet<ChannelId, ChannelIdHash> unknown_channels_;
  WaitFreeHashSet<ChannelId, ChannelIdHash> invalidated_channels_full_;
  WaitFreeHashMap<ChannelId, FileSourceId, ChannelIdHash> channel_full_file_source_ids_;

  WaitFreeHashMap<SecretChatId, unique_ptr<SecretChat>, SecretChatIdHash> secret_chats_;
  mutable FlatHashSet<SecretChatId, SecretChatIdHash> unknown_secret_chats_;

  FlatHashMap<UserId, vector<SecretChatId>, UserIdHash> secret_chats_with_user_;

  FlatHashMap<ChannelId, RecommendedDialogs, ChannelIdHash> channel_recommended_dialogs_;
  FlatHashMap<ChannelId, vector<Promise<td_api::object_ptr<td_api::chats>>>, ChannelIdHash>
      get_channel_recommendations_queries_;
  FlatHashMap<ChannelId, vector<Promise<td_api::object_ptr<td_api::count>>>, ChannelIdHash>
      get_channel_recommendation_count_queries_[2];

  bool created_public_channels_inited_[2] = {false, false};
  vector<ChannelId> created_public_channels_[2];
  vector<Promise<td_api::object_ptr<td_api::chats>>> get_created_public_channels_queries_[2];

  bool dialogs_for_discussion_inited_ = false;
  vector<DialogId> dialogs_for_discussion_;

  bool inactive_channel_ids_inited_ = false;
  vector<ChannelId> inactive_channel_ids_;

  FlatHashMap<UserId, vector<Promise<Unit>>, UserIdHash> load_user_from_database_queries_;
  FlatHashSet<UserId, UserIdHash> loaded_from_database_users_;
  FlatHashSet<UserId, UserIdHash> unavailable_user_fulls_;

  FlatHashMap<ChatId, vector<Promise<Unit>>, ChatIdHash> load_chat_from_database_queries_;
  FlatHashSet<ChatId, ChatIdHash> loaded_from_database_chats_;
  FlatHashSet<ChatId, ChatIdHash> unavailable_chat_fulls_;

  FlatHashMap<ChannelId, vector<Promise<Unit>>, ChannelIdHash> load_channel_from_database_queries_;
  FlatHashSet<ChannelId, ChannelIdHash> loaded_from_database_channels_;
  FlatHashSet<ChannelId, ChannelIdHash> unavailable_channel_fulls_;

  FlatHashMap<SecretChatId, vector<Promise<Unit>>, SecretChatIdHash> load_secret_chat_from_database_queries_;
  FlatHashSet<SecretChatId, SecretChatIdHash> loaded_from_database_secret_chats_;

  QueryMerger get_user_queries_{"GetUserMerger", 3, 50};
  QueryMerger get_chat_queries_{"GetChatMerger", 3, 50};
  QueryMerger get_channel_queries_{"GetChannelMerger", 100, 1};  // can't merge getChannel queries without access hash

  QueryMerger get_is_premium_required_to_contact_queries_{"GetIsPremiumRequiredToContactMerger", 3, 100};

  QueryCombiner get_user_full_queries_{"GetUserFullCombiner", 2.0};
  QueryCombiner get_chat_full_queries_{"GetChatFullCombiner", 2.0};

  FlatHashMap<DialogId, vector<SuggestedAction>, DialogIdHash> dialog_suggested_actions_;
  FlatHashMap<DialogId, vector<Promise<Unit>>, DialogIdHash> dismiss_suggested_action_queries_;

  class UploadProfilePhotoCallback;
  std::shared_ptr<UploadProfilePhotoCallback> upload_profile_photo_callback_;

  struct UploadedProfilePhoto {
    UserId user_id;
    bool is_fallback;
    bool only_suggest;
    double main_frame_timestamp;
    bool is_animation;
    int reupload_count;
    Promise<Unit> promise;

    UploadedProfilePhoto(UserId user_id, bool is_fallback, bool only_suggest, double main_frame_timestamp,
                         bool is_animation, int32 reupload_count, Promise<Unit> promise)
        : user_id(user_id)
        , is_fallback(is_fallback)
        , only_suggest(only_suggest)
        , main_frame_timestamp(main_frame_timestamp)
        , is_animation(is_animation)
        , reupload_count(reupload_count)
        , promise(std::move(promise)) {
    }
  };
  FlatHashMap<FileId, UploadedProfilePhoto, FileIdHash> uploaded_profile_photos_;

  struct ImportContactsTask {
    Promise<Unit> promise_;
    vector<Contact> input_contacts_;
    vector<UserId> imported_user_ids_;
    vector<int32> unimported_contact_invites_;
  };
  FlatHashMap<int64, unique_ptr<ImportContactsTask>> import_contact_tasks_;

  FlatHashMap<int64, std::pair<vector<UserId>, vector<int32>>> imported_contacts_;

  struct UserOnlineMemberDialogs {
    FlatHashMap<DialogId, int32, DialogIdHash> online_member_dialogs_;  // dialog_id -> time
  };
  FlatHashMap<UserId, unique_ptr<UserOnlineMemberDialogs>, UserIdHash> user_online_member_dialogs_;

  FlatHashMap<ChannelId, vector<DialogParticipant>, ChannelIdHash> cached_channel_participants_;

  FlatHashMap<string, UserId> resolved_phone_numbers_;

  FlatHashMap<UserId, FlatHashSet<MessageFullId, MessageFullIdHash>, UserIdHash> user_messages_;
  FlatHashMap<ChannelId, FlatHashSet<MessageFullId, MessageFullIdHash>, ChannelIdHash> channel_messages_;

  bool are_contacts_loaded_ = false;
  int32 next_contacts_sync_date_ = 0;
  Hints contacts_hints_;  // search contacts by first name, last name and usernames
  vector<Promise<Unit>> load_contacts_queries_;
  MultiPromiseActor load_contact_users_multipromise_{"LoadContactUsersMultiPromiseActor"};
  int32 saved_contact_count_ = -1;

  int32 was_online_local_ = 0;
  int32 was_online_remote_ = 0;

  bool are_imported_contacts_loaded_ = false;
  vector<Promise<Unit>> load_imported_contacts_queries_;
  MultiPromiseActor load_imported_contact_users_multipromise_{"LoadImportedContactUsersMultiPromiseActor"};
  vector<Contact> all_imported_contacts_;
  bool are_imported_contacts_changing_ = false;
  bool need_clear_imported_contacts_ = false;

  vector<DialogNearby> users_nearby_;
  vector<DialogNearby> channels_nearby_;
  FlatHashSet<UserId, UserIdHash> all_users_nearby_;

  int32 location_visibility_expire_date_ = 0;
  int32 pending_location_visibility_expire_date_ = -1;
  bool is_set_location_visibility_request_sent_ = false;
  Location last_user_location_;

  FlatHashMap<UserId, bool, UserIdHash> user_full_contact_require_premium_;

  WaitFreeHashMap<ChannelId, ChannelId, ChannelIdHash> linked_channel_ids_;

  WaitFreeHashSet<UserId, UserIdHash> restricted_user_ids_;
  WaitFreeHashSet<ChannelId, ChannelIdHash> restricted_channel_ids_;

  vector<Contact> next_all_imported_contacts_;
  vector<size_t> imported_contacts_unique_id_;
  vector<size_t> imported_contacts_pos_;

  vector<UserId> imported_contact_user_ids_;  // result of change_imported_contacts
  vector<int32> unimported_contact_invites_;  // result of change_imported_contacts

  MultiTimeout user_online_timeout_{"UserOnlineTimeout"};
  MultiTimeout user_emoji_status_timeout_{"UserEmojiStatusTimeout"};
  MultiTimeout channel_emoji_status_timeout_{"ChannelEmojiStatusTimeout"};
  MultiTimeout channel_unban_timeout_{"ChannelUnbanTimeout"};
  MultiTimeout user_nearby_timeout_{"UserNearbyTimeout"};
  MultiTimeout slow_mode_delay_timeout_{"SlowModeDelayTimeout"};
};

}  // namespace td
