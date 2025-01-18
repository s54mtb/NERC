/**
 * @file network.h
 * @brief Definitions of network notification flags.
 *
 * This header file defines constants for network notifications such as
 * link down, link up, wakeup, and timer alarm events.
 *
 * @author S54MTB, Marko Pavlin
 * @date January 2025
 */

#ifndef NETWORK_H
#define NETWORK_H

#ifdef __cplusplus
extern "C" {
#endif

// Network notification flags
#define NET_FL_NOTIFY_LINK_DOWN    (1U << 0)     /**< Link down notification flag */
#define NET_FL_NOTIFY_LINK_UP      (1U << 1)     /**< Link up notification flag */
#define NET_FL_NOTIFY_WAKEUP       (1U << 2)     /**< Wakeup notification flag */
#define NET_FL_NOTIFY_TIMER_ALARM  (1U << 3)     /**< Timer alarm notification flag */
#define NET_FL_NOTIFY_UDP_RECEIVED (1U << 4)     /**< New position UDP received */

#ifdef __cplusplus
}
#endif

#endif // NETWORK_H
