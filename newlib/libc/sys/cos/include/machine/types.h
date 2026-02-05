/*
 * Newlib targets may provide an own version of this file in their machine
 * directory to add custom user types for <sys/types.h>.
 *
 * Copyright 2023
 * Kalray Inc. All rights reserved.
 *
 */
#ifndef _SYS_TYPES_H
#error "must be included via <sys/types.h>"
#endif /* !_SYS_TYPES_H */

#ifndef _SYS_MACHINE_COS_TYPES_H
#define _SYS_MACHINE_COS_TYPES_H

typedef	__uint64_t	u_quad_t;
typedef	__int64_t	quad_t;
typedef	quad_t *	qaddr_t;

#endif
