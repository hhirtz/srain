#ifndef __UI_H
#define __UI_H

#include <glib.h>
#include <irc.h>
#include <srain_window.h>

#define PATH_LEN 256

typedef struct {
    char name[CHAN_LEN];
} chan_name_t;

typedef struct {
    char chan[CHAN_LEN];
    char nick[NICK_LEN];
} online_list_item_t;

typedef struct {
    char chan[CHAN_LEN];
    char topic[MSG_LEN];
} topic_t;

typedef struct {
    char id[NICK_LEN];
    char nick[NICK_LEN];
    char chan[CHAN_LEN];
    char msg[MSG_LEN];
    char avatar[PATH_LEN];   // path of cached avatar, can be null
    char img[PATH_LEN];      // path of cached img, can be null
} recv_msg_t;

typedef struct {
    char chan[CHAN_LEN];
    char msg[MSG_LEN];
} sys_msg_t;

void ui_init(SrainWindow *swin);

void ui_chan_add(const char *chan_name);
void ui_chan_rm(const char *chan_name);

void ui_msg_sys(const char *chan_name, const char *msg);
void ui_msg_sysf(const char *chan_name, const char *fmt, ...);
void ui_msg_sysf(const char *chan_name, const char *fmt, ...);
void ui_msg_sysf_broadcast(GList *chans, const char *fmt, ...);

void ui_msg_send(const char *chan_name, const char *msg);

void ui_msg_recv(const char *chan_name, const char *nick, const char *id, const char *msg);
void ui_msg_recv_broadcast(GList *chans, const char *nick, const char *id, const char *msg);

void ui_chan_online_list_add(const char *chan_name, const char *name, int is_init);
void ui_chan_online_list_rm(const char *chan_name, const char *name, const char *reason);
void ui_chan_online_list_rm_broadcast(GList *chans, const char *name, const char *reason);
void ui_chan_online_list_rename_broadcast(GList *chans, const char *old_name, const char *new_name);

const char* ui_chan_get_cur_name();
void ui_busy(gboolean is_busy);
void ui_chan_set_topic(const char *chan_name, const char *topic);

#endif /* __UI_H */
