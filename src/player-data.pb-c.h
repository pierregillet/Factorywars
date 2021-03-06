/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: player-data.proto */

#ifndef PROTOBUF_C_player_2ddata_2eproto__INCLUDED
#define PROTOBUF_C_player_2ddata_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1000000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1002001 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _PlayerData PlayerData;


/* --- enums --- */


/* --- messages --- */

struct  _PlayerData
{
  ProtobufCMessage base;
  char *name;
  int32_t health;
  int64_t x;
  int64_t y;
  int32_t velocity;
  size_t n_inventory_item_id;
  int32_t *inventory_item_id;
  size_t n_inventory_number;
  int32_t *inventory_number;
  size_t n_toolbar_item_id;
  int32_t *toolbar_item_id;
  size_t n_toolbar_number;
  int32_t *toolbar_number;
  int32_t selected_tool;
};
#define PLAYER_DATA__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&player_data__descriptor) \
    , NULL, 0, 0, 0, 0, 0,NULL, 0,NULL, 0,NULL, 0,NULL, 0 }


/* PlayerData methods */
void   player_data__init
                     (PlayerData         *message);
size_t player_data__get_packed_size
                     (const PlayerData   *message);
size_t player_data__pack
                     (const PlayerData   *message,
                      uint8_t             *out);
size_t player_data__pack_to_buffer
                     (const PlayerData   *message,
                      ProtobufCBuffer     *buffer);
PlayerData *
       player_data__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   player_data__free_unpacked
                     (PlayerData *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*PlayerData_Closure)
                 (const PlayerData *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor player_data__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_player_2ddata_2eproto__INCLUDED */
