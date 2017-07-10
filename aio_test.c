// Copyright (c) 2017, eaugeas <eaugeas at gmail dot com>

#include <stdio.h>

#include "aio.h"
#include "test.h"

static int test_calls() {
  aqevent_t events[1];
  memset(&events[0], 0, sizeof(aqevent_t));
  int queuefd = aqcreate();

  ASSERT_NEQ(queuefd, -1);
  ASSERT_EQ(aqwait(queuefd, events, 1, 1), 0);
  ASSERT_EQ(0, aqclose(queuefd));

  ASSERT_NULL(aqget(&events[0]));

  ASSERT_FALSE(aqis_closed(&events[0]));
  ASSERT_FALSE(aqis_peer_closed(&events[0]));
  ASSERT_FALSE(aqis_error(&events[0]));
  ASSERT_FALSE(aqis_read(&events[0]));
  ASSERT_FALSE(aqis_write(&events[0]));

  return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
  test_ctx_t ctx;

  TEST_INIT(ctx, argc, argv);

  TEST_RUN(ctx, test_calls);

  TEST_RELEASE(ctx);

  return EXIT_SUCCESS;

}
