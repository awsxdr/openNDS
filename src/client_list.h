/********************************************************************\
 * This program is free software; you can redistribute it and/or    *
 * modify it under the terms of the GNU General Public License as   *
 * published by the Free Software Foundation; either version 2 of   *
 * the License, or (at your option) any later version.              *
 *                                                                  *
 * This program is distributed in the hope that it will be useful,  *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of   *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the    *
 * GNU General Public License for more details.                     *
 *                                                                  *
 * You should have received a copy of the GNU General Public License*
 * along with this program; if not, contact:                        *
 *                                                                  *
 * Free Software Foundation           Voice:  +1-617-542-5942       *
 * 59 Temple Place - Suite 330        Fax:    +1-617-542-2652       *
 * Boston, MA  02111-1307,  USA       gnu@gnu.org                   *
 *                                                                  *
\********************************************************************/

/** @file client_list.h
    @brief Client List functions
    @author Copyright (C) 2004 Alexandre Carmel-Veilleux <acv@acv.ca>
    @author Copyright (C) 2007 Paul Kube <nodogsplash@kokoro.ucsd.edu>
    @author Copyright (C) 2015-2025 Modifications and additions by BlueWave Projects and Services <opennds@blue-wave.net>
*/

#ifndef _CLIENT_LIST_H_
#define _CLIENT_LIST_H_

/** Counters struct for a client's bandwidth usage (in bytes)
 */
typedef struct _t_counters {
	unsigned long long int incoming;		/**< @brief Incoming bytes total */
	unsigned long long int incoming_previous;	/**< @brief Incoming bytes previous */
	unsigned long long int outgoing;		/**< @brief Outgoing bytes total */
	unsigned long long int outgoing_previous;	/**< @brief Outgoing bytes previous */
	unsigned long long int inpackets;		/**< @brief Incoming packets total */
	unsigned long long int inpackets_previous;	/**< @brief Incoming packets previous */
	unsigned long long int outpackets;		/**< @brief Outgoing packets total */
	unsigned long long int outpackets_previous;	/**< @brief Outgoing packets previous */
	unsigned long long int in_window_start;	/**< @brief Incoming bytes total at rate check window start */
	unsigned long long int out_window_start;	/**< @brief Outgoing bytes total at rate check window start */
	time_t last_updated;				/**< @brief Last update of the counters */
} t_counters;

/** Client node for the connected client linked list.
 */
typedef struct _t_client {
	struct _t_client *next;				/**< @brief Pointer to the next client */
	char *ip;					/**< @brief Client IP address */
	char *mac;					/**< @brief Client MAC address */
	char *token;					/**< @brief Client token */
	char *hid;					/**< @brief Client hid */
	char *cid;					/**< @brief Client cid */
	char *custom;					/**< @brief Client custom string sent from FAS and sent to BinAuth */
	char *client_type;				/**< @brief Client type, cpd (cpd_can), rfc8910-cpi (cpi_url) or rfc8908-cpi (cpi_api)  */
	char *cpi_query;				/**< @brief RFC8910-cpi query string  */
	unsigned int fw_connection_state;		/**< @brief Client Connection state in the firewall */
	time_t session_start;				/**< @brief Actual Time the client was authenticated */
	time_t window_start;				/**< @brief Actual Time the client rate check window begins */
	time_t session_end;				/**< @brief Scheduled Time the client will be deauthenticated */
	t_counters counters;				/**< @brief Counters for input/output of the client. */
	int window_counter;				/**< @brief Rate Check Window counter */
	int rate_exceeded;				/**< @brief Rate Exceeded Check flag */
	int initial_loop;				/**< @brief Check client initial loop flag */
	int upload_limiting;				/**< @brief Limiting status flag */
	int download_limiting;				/**< @brief Limiting status flag */
	unsigned long long int upload_rate;		/**< @brief Client Upload rate limit, kb/s */
	unsigned long long int download_rate;		/**< @brief Client Download rate limit, kb/s */
	unsigned long long int uprate;			/**< @brief Current Client Upload rate, kb/s */
	unsigned long long int downrate;		/**< @brief Client Download rate, kb/s */
	unsigned long long int upload_quota;		/**< @brief Client Upload quota, kB */
	unsigned long long int download_quota;		/**< @brief Client Download quota, kB */
	unsigned long long int download_bucket_size;	/**< @brief Client Download Bucket size, packets */
	unsigned long long int upload_bucket_size;	/**< @brief Client Upload Bucket size, packets */
	unsigned long long int inc_packet_limit;	/**< @brief Incoming packet limit */
	unsigned long long int out_packet_limit;	/**< @brief Outgoing packet limit */
	unsigned id;
} t_client;

/** @brief Get the first element of the list of connected clients
 */
t_client *client_get_first_client(void);

/** @brief Initializes the client list */
void client_list_init(void);

/** @brief Returns number of clients currently on client list */
int get_client_list_length();

/** @brief Adds a new client to the client list */
t_client *client_list_add_client(const char mac[], const char ip[]);

/** @brief Finds a client by its MAC, IP or token */
t_client *client_list_find_by_any(const char mac[], const char ip[], const char token[]);

/** @brief Finds a client by its MAC and IP */
t_client * client_list_find(const char mac[], const char ip[]);

/** @brief Finds a client by its client id */
t_client * client_list_find_by_id(const unsigned id);

/** @brief Finds a client only by its IP */
t_client *client_list_find_by_ip(const char ip[]); /* needed by fw_iptables.c, auth.c * and ndsctl_thread.c */

/** @brief Finds a client only by its MAC */
t_client *client_list_find_by_mac(const char mac[]); /* needed by ndsctl_thread.c */

/** @brief Finds a client by its token */
t_client *client_list_find_by_token(const char token[]);

/** @brief Deletes a client from the client list */
void client_list_delete(t_client *client);

#define LOCK_CLIENT_LIST() do { \
	debug(LOG_DEBUG, "Locking client list"); \
	pthread_mutex_lock(&client_list_mutex); \
	debug(LOG_DEBUG, "Client list locked"); \
} while (0)

#define UNLOCK_CLIENT_LIST() do { \
	debug(LOG_DEBUG, "Unlocking client list"); \
	pthread_mutex_unlock(&client_list_mutex); \
	debug(LOG_DEBUG, "Client list unlocked"); \
} while (0)

extern pthread_mutex_t client_list_mutex;

#endif /* _CLIENT_LIST_H_ */
