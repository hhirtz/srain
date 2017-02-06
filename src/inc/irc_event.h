/*
 * Copyright (C) 2004-2012 George Yunaev <gyunaev@ulduzsoft.com>
 * Copyright (C) 2017 Shengyu Zhang <silverainz@outlook.com>
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 */

/* This file is borrowed from libircclient(include/libirc_events.h) */

#ifndef __IRC_EVENT_H
#define __IRC_EVENT_H

#include "irc.h"
#include "irc_parse.h"

/*!
 * \fn typedef void (*IRCEventFunc) (Server * srv, const char * event, const char * origin, const char ** params, unsigned int count)
 * \brief A most common event callback
 *
 * \param session the session, which generates an event
 * \param event   the text name of the event. Useful in case you use a single
 *                event handler for several events simultaneously.
 * \param origin  the originator of the event. See the note below.
 * \param params  a list of event params. Depending on the event nature, it
 *                could have zero or more params. The actual number of params
 *                is specified in count. None of the params can be NULL, but
 *                'params' pointer itself could be NULL for some events.
 * \param count   the total number of params supplied.
 *
 * Every event generates a callback. This callback is generated by most events.
 * Depending on the event nature, it can provide zero or more params. For each
 * event, the number of provided params is fixed, and their meaning is
 * described.
 *
 * Every event has origin, though the \a origin variable may be NULL, which
 * means that event origin is unknown. The origin usually looks like
 * nick!host\@ircserver, i.e. like tim!home\@irc.krasnogorsk.ru. Such origins
 * can not be used in IRC commands, and need to be stripped (i.e. host and
 * server part should be cut off) before using. This can be done either
 * explicitly, by calling irc_target_get_nick(), or implicitly for all the
 * events - by setting the #LIBIRC_OPTION_STRIPNICKS option with irc_option_set().
 *
 * \ingroup events
 */
typedef void (*IRCEventFunc) (Server * srv, const char * event, const char * origin, const char ** params, unsigned int count);


/*!
 * \fn typedef void (*IRCEventCodeFunc) (Server * srv, unsigned int event, const char * origin, const char ** params, unsigned int count)
 * \brief A numeric event callback
 *
 * \param session the session, which generates an event
 * \param event   the numeric code of the event. Useful in case you use a
 *                single event handler for several events simultaneously.
 * \param origin  the originator of the event. See the note below.
 * \param params  a list of event params. Depending on the event nature, it
 *                could have zero or more params. The actual number of params
 *                is specified in count. None of the params can be NULL, but
 *                'params' pointer itself could be NULL for some events.
 * \param count   the total number of params supplied.
 *
 * Most times in reply to your actions the IRC server generates numeric
 * callbacks. Most of them are error codes, and some of them mark list start
 * and list stop markers. Every code has its own set of params; for details
 * you can either experiment, or read RFC 1459.
 *
 * Every event has origin, though the \a origin variable may be NULL, which
 * means that event origin is unknown. The origin usually looks like
 * nick!host\@ircserver, i.e. like tim!home\@irc.krasnogorsk.ru. Such origins
 * can not be used in IRC commands, and need to be stripped (i.e. host and
 * server part should be cut off) before using. This can be done either
 * explicitly, by calling irc_target_get_nick(), or implicitly for all the
 * events - by setting the #LIBIRC_OPTION_STRIPNICKS option with irc_option_set().
 *
 * \ingroup events
 */
typedef void (*IRCEventCodeFunc) (Server * srv, unsigned int event, const char * origin, const char ** params, unsigned int count);

/*! \brief Event callbacks structure.
 *
 * All the communication with the IRC network is based on events. Generally
 * speaking, event is anything generated by someone else in the network,
 * or by the IRC server itself. "Someone sends you a message", "Someone
 * has joined the channel", "Someone has quits IRC" - all these messages
 * are events.
 *
 * Every event has its own event handler, which is called when the
 * appropriate event is received. You don't have to define all the event
 * handlers; define only the handlers for the events you need to intercept.
 *
 * Most event callbacks are the types of ::IRCEventFunc. There are
 * also events, which generate ::IRCEventCodeFunc,
 * ::irc_dcc_chat_t and ::irc_event_dcc_send_t callbacks.
 *
 * \ingroup events
 */
typedef struct {
    /*!
     * The "on_connect" event is triggered when the client successfully
     * connects to the server, and could send commands to the server.
     * No extra params supplied; \a params is 0.
     */
    IRCEventFunc    connect;

    /*!
     * The "on_ping" event is triggered when the client receives a PING
     *message from server, you do not neet to return a PONG message.
     *
     * \param origin the server, who sent PING message.
     */
    IRCEventFunc    ping;

    /*!
     * The "nick" event is triggered when the client receives a NICK message,
     * meaning that someone (including you) on a channel with the client has
     * changed their nickname.
     *
     * \param origin the person, who changes the nick. Note that it can be you!
     * \param params[0] mandatory, contains the new nick.
     */
    IRCEventFunc    nick;

    /*!
     * The "quit" event is triggered upon receipt of a QUIT message, which
     * means that someone on a channel with the client has disconnected.
     *
     * \param origin the person, who is disconnected
     * \param params[0] optional, contains the reason message (user-specified).
     */
    IRCEventFunc    quit;

    /*!
     * The "join" event is triggered upon receipt of a JOIN message, which
     * means that someone has entered a channel that the client is on.
     *
     * \param origin the person, who joins the channel. By comparing it with
     *               your own nickname, you can check whether your JOIN
     *               command succeed.
     * \param params[0] mandatory, contains the channel name.
     */
    IRCEventFunc    join;

    /*!
     * The "part" event is triggered upon receipt of a PART message, which
     * means that someone has left a channel that the client is on.
     *
     * \param origin the person, who leaves the channel. By comparing it with
     *               your own nickname, you can check whether your PART
     *               command succeed.
     * \param params[0] mandatory, contains the channel name.
     * \param params[1] optional, contains the reason message (user-defined).
     */
    IRCEventFunc    part;

    /*!
     * The "mode" event is triggered upon receipt of a channel MODE message,
     * which means that someone on a channel with the client has changed the
     * channel's parameters.
     *
     * \param origin the person, who changed the channel mode.
     * \param params[0] mandatory, contains the channel name.
     * \param params[1] mandatory, contains the changed channel mode, like
     *        '+t', '-i' and so on.
     * \param params[2] optional, contains the mode argument (for example, a
     *      key for +k mode, or user who got the channel operator status for
     *      +o mode)
     */
    IRCEventFunc    mode;

    /*!
     * The "umode" event is triggered upon receipt of a user MODE message,
     * which means that your user mode has been changed.
     *
     * \param origin the person, who changed the channel mode.
     * \param params[0] mandatory, contains the user changed mode, like
     *        '+t', '-i' and so on.
     */
    IRCEventFunc    umode;

    /*!
     * The "topic" event is triggered upon receipt of a TOPIC message, which
     * means that someone on a channel with the client has changed the
     * channel's topic.
     *
     * \param origin the person, who changes the channel topic.
     * \param params[0] mandatory, contains the channel name.
     * \param params[1] optional, contains the new topic.
     */
    IRCEventFunc    topic;

    /*!
     * The "kick" event is triggered upon receipt of a KICK message, which
     * means that someone on a channel with the client (or possibly the
     * client itself!) has been forcibly ejected.
     *
     * \param origin the person, who kicked the poor.
     * \param params[0] mandatory, contains the channel name.
     * \param params[0] optional, contains the nick of kicked person.
     * \param params[1] optional, contains the kick text
     */
    IRCEventFunc    kick;

    /*!
     * The "channel" event is triggered upon receipt of a PRIVMSG message
     * to an entire channel, which means that someone on a channel with
     * the client has said something aloud. Your own messages don't trigger
     * PRIVMSG event.
     *
     * \param origin the person, who generates the message.
     * \param params[0] mandatory, contains the channel name.
     * \param params[1] optional, contains the message text
     */
    IRCEventFunc    channel;

    /*!
     * The "privmsg" event is triggered upon receipt of a PRIVMSG message
     * which is addressed to one or more clients, which means that someone
     * is sending the client a private message.
     *
     * \param origin the person, who generates the message.
     * \param params[0] mandatory, contains your nick.
     * \param params[1] optional, contains the message text
     */
    IRCEventFunc    privmsg;

    /*!
     * The "notice" event is triggered upon receipt of a NOTICE message
     * which means that someone has sent the client a public or private
     * notice. According to RFC 1459, the only difference between NOTICE
     * and PRIVMSG is that you should NEVER automatically reply to NOTICE
     * messages. Unfortunately, this rule is frequently violated by IRC
     * servers itself - for example, NICKSERV messages require reply, and
     * are NOTICEs.
     *
     * \param origin the person, who generates the message.
     * \param params[0] mandatory, contains the target nick name.
     * \param params[1] optional, contains the message text
     */
    IRCEventFunc    notice;

    /*!
     * The "channel_notice" event is triggered upon receipt of a NOTICE
     * message which means that someone has sent the client a public
     * notice. According to RFC 1459, the only difference between NOTICE
     * and PRIVMSG is that you should NEVER automatically reply to NOTICE
     * messages. Unfortunately, this rule is frequently violated by IRC
     * servers itself - for example, NICKSERV messages require reply, and
     * are NOTICEs.
     *
     * \param origin the person, who generates the message.
     * \param params[0] mandatory, contains the channel name.
     * \param params[1] optional, contains the message text
     */
    IRCEventFunc    channel_notice;

    /*!
     * The "invite" event is triggered upon receipt of an INVITE message,
     * which means that someone is permitting the client's entry into a +i
     * channel.
     *
     * \param origin the person, who INVITEs you.
     * \param params[0] mandatory, contains your nick.
     * \param params[1] mandatory, contains the channel name you're invited into.
     *
     * \sa irc_cmd_invite irc_cmd_chanmode_invite
     */
    IRCEventFunc    invite;

    /*!
     * The "ctcp" event is triggered when the client receives the CTCP
     * request. By default, the built-in CTCP request handler is used. The
     * build-in handler automatically replies on most CTCP messages, so you
     * will rarely need to override it.
     *
     * \param origin the person, who generates the message.
     * \param params[0] mandatory, the complete CTCP message, including its
     *                  arguments.
     *
     * Mirc generates PING, FINGER, VERSION, TIME and ACTION messages,
     * check the source code of \c libirc_ctcp_internal function to
     * see how to write your own CTCP request handler. Also you may find
     * useful this question in FAQ: \ref faq4
     */
    IRCEventFunc    ctcp_req;

    /*!
     * The "ctcp" event is triggered when the client receives the CTCP reply.
     *
     * \param origin the person, who generates the message.
     * \param params[0] mandatory, the CTCP message itself with its arguments.
     */
    IRCEventFunc    ctcp_rep;

    /*!
     * The "action" event is triggered when the client receives the CTCP
     * ACTION message. These messages usually looks like:\n
     * \code
     * [23:32:55] * Tim gonna sleep.
     * \endcode
     *
     * \param origin the person, who generates the message.
     * \param params[0] mandatory, the ACTION message.
     */
    IRCEventFunc    ctcp_action;

    /*!
     * The "unknown" event is triggered upon receipt of any number of
     * unclassifiable miscellaneous messages, which aren't handled by the
     * library.
     */
    IRCEventFunc    unknown;

    /*!
     * The "numeric" event is triggered upon receipt of any numeric response
     * from the server. There is a lot of such responses, see the full list
     * here: \ref rfcnumbers.
     *
     * See the params in ::IRCEventCodeFunc specification.
     */
    IRCEventCodeFunc    numeric;

} IRCEvents;

void irc_event_init();
void irc_event_hdr(Server *srv, IRCMsg *imsg);

#endif /* __IRC_EVENT_H */
