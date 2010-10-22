/*
 * Copyright (C) 2010 Red Hat, Inc.
 *
 * Author: Angus Salkeld <asalkeld@redhat.com>
 *
 * This file is part of libqb.
 *
 * libqb is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * libqb is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libqb.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef QB_LOOP_DEFINED
#define QB_LOOP_DEFINED

/**
 * @file qbloop.h
 *
 * Main loop manages timers, jobs and polling sockets.
 */


/**
 * Priorites for jobs, timers & poll
 */
enum qb_loop_priority {
	QB_LOOP_LOW = 0,
	QB_LOOP_MED = 1,
	QB_LOOP_HIGH = 2,
};

/**
 * An opaque data type representing the main loop.
 */
typedef struct qb_loop qb_loop_t;

typedef void *qb_loop_job_handle;
typedef void *qb_loop_timer_handle;

typedef int32_t (*qb_loop_poll_dispatch_fn) (int32_t fd, int32_t revents, void *data);
typedef void (*qb_loop_job_dispatch_fn)(void *data);
typedef void (*qb_loop_timer_dispatch_fn)(void *data);

typedef void (*qb_loop_poll_low_fds_event_fn) (int32_t not_enough, int32_t fds_available);

/**
 * Create a new main loop.
 * 
 * @return loop instance.
 */
qb_loop_t * qb_loop_create(void);

/**
 * Stop the main loop.
 * @param l pointer to the loop instance
 */
void qb_loop_stop(qb_loop_t *l);

/**
 * Run the main loop.
 *
 * @param l pointer to the loop instance
 */
void qb_loop_run(qb_loop_t *l);


/**
 * Add a job to the mainloop.
 *
 * This is run in the next cycle of the loop.
 * @note it is a one-shot job.
 *
 * @param l pointer to the loop instance
 * @param p the priority
 * @param data user data passed into the dispatch function
 * @param dispatch_fn callback function
 * @return status (0 == ok, -errno == failure)
 */
int32_t qb_loop_job_add(qb_loop_t *l,
			enum qb_loop_priority p,
			void *data,
			qb_loop_job_dispatch_fn dispatch_fn);

/**
 * Add a timer to the mainloop.
 * @note it is a one-shot job.
 *
 * @param l pointer to the loop instance
 * @param p the priority
 * @param msec_duration milli-secs in the future to run the dispatch.
 * @param data user data passed into the dispatch function
 * @param dispatch_fn callback function
 * @param timer_handle_out handle to delete the timer if needed.
 * @return status (0 == ok, -errno == failure)
 */
int32_t qb_loop_timer_add(qb_loop_t *l,
			  enum qb_loop_priority p,
			  int32_t msec_duration,
			  void *data,
			  qb_loop_timer_dispatch_fn dispatch_fn,
			  qb_loop_timer_handle * timer_handle_out);

/**
 * Delete a timer that is still outstanding.
 *
 * @param l pointer to the loop instance
 * @param th handle to delete the timer if needed.
 * @return status (0 == ok, -errno == failure)
 */
int32_t qb_loop_timer_del(qb_loop_t *l, qb_loop_timer_handle th);

/**
 * Set a callback to receive events on file descriptors
 * getting low.
 * @param l pointer to the loop instance
 * @param fn callback function.
 * @return status (0 == ok, -errno == failure)
 */
int32_t qb_loop_poll_low_fds_event_set(qb_loop_t *l,
				       qb_loop_poll_low_fds_event_fn fn);



/**
 * Add a poll job to the mainloop.
 * @note it is a re-occuring job.
 *
 * @param l pointer to the loop instance
 * @param p the priority
 * @param fd file descriptor.
 * @param events (POLLIN|POLLOUT) etc ....
 * @param data user data passed into the dispatch function
 * @param dispatch_fn callback function
 * @return status (0 == ok, -errno == failure)
 */
int32_t qb_loop_poll_add(qb_loop_t *l,
			     enum qb_loop_priority p,
			     int32_t fd,
			     int32_t events,
			     void *data,
			     qb_loop_poll_dispatch_fn dispatch_fn);

/**
 * Modify a poll job.
 *
 * @param l pointer to the loop instance
 * @param p the priority
 * @param fd file descriptor.
 * @param events (POLLIN|POLLOUT) etc ....
 * @param data user data passed into the dispatch function
 * @param dispatch_fn callback function
 * @return status (0 == ok, -errno == failure)
 */
int32_t qb_loop_poll_mod(qb_loop_t *l,
			 enum qb_loop_priority p,
			 int32_t fd,
			 int32_t events,
			 void *data,
			 qb_loop_poll_dispatch_fn dispatch_fn);

/**
 * Delete a poll job.
 *
 * @param l pointer to the loop instance
 * @param fd file descriptor.
 * @return status (0 == ok, -errno == failure)
 */
int32_t qb_loop_poll_del(qb_loop_t *l, int32_t fd);

#endif /* QB_LOOP_DEFINED */
