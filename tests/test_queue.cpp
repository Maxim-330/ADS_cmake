// Copyright 2026 Maksim Onotin

#include <gtest/gtest.h>

#include <stdexcept>
#include <string>

#include "queue.h"

TEST(QueueBasic, EmptyOnCreation) {
  Queue<int> q(3);
  EXPECT_TRUE(q.is_empty());
  EXPECT_FALSE(q.is_full());
  EXPECT_EQ(q.size(), 0u);
}

TEST(QueueBasic, PushPop) {
  Queue<int> q(3);
  q.push(10);
  EXPECT_FALSE(q.is_empty());
  EXPECT_EQ(q.size(), 1u);
  EXPECT_EQ(q.front(), 10);

  q.push(20);
  q.push(30);
  EXPECT_TRUE(q.is_full());
  EXPECT_EQ(q.size(), 3u);
  EXPECT_EQ(q.front(), 10);

  EXPECT_EQ(q.pop(), 10);
  EXPECT_EQ(q.pop(), 20);
  EXPECT_EQ(q.pop(), 30);
  EXPECT_TRUE(q.is_empty());
  EXPECT_EQ(q.size(), 0u);
}

TEST(QueueCircular, WrapAround) {
  Queue<int> q(3);
  q.push(1);
  q.push(2);
  q.push(3);
  EXPECT_EQ(q.pop(), 1);
  q.push(4);
  EXPECT_EQ(q.pop(), 2);
  EXPECT_EQ(q.pop(), 3);
  EXPECT_EQ(q.pop(), 4);
  EXPECT_TRUE(q.is_empty());
}

TEST(QueueExceptions, PushToFull) {
  Queue<int> q(2);
  q.push(1);
  q.push(2);
  EXPECT_THROW(q.push(3), std::overflow_error);
}

TEST(QueueExceptions, PopFromEmpty) {
  Queue<int> q(2);
  EXPECT_THROW(q.pop(), std::underflow_error);
}

TEST(QueueExceptions, FrontOnEmpty) {
  Queue<int> q(2);
  EXPECT_THROW(q.front(), std::underflow_error);
}

TEST(QueueExceptions, ConstFrontOnEmpty) {
  const Queue<int> q(2);
  EXPECT_THROW(q.front(), std::underflow_error);
}

TEST(QueueExceptions, ZeroCapacity) {
  EXPECT_THROW(Queue<int>(0), std::invalid_argument);
}

TEST(QueueCopy, CopyEmpty) {
  Queue<int> a(3);
  Queue<int> b = a;
  EXPECT_TRUE(b.is_empty());
  EXPECT_EQ(b.size(), 0u);
}

TEST(QueueCopy, CopyFull) {
  Queue<int> a(3);
  a.push(1);
  a.push(2);
  a.push(3);
  Queue<int> b = a;
  EXPECT_EQ(b.size(), 3u);
  EXPECT_TRUE(b.is_full());
  EXPECT_EQ(b.pop(), 1);
  EXPECT_EQ(b.pop(), 2);
  EXPECT_EQ(b.pop(), 3);
}

TEST(QueueCopy, CopyWrapped) {
  Queue<int> a(3);
  a.push(1);
  a.push(2);
  a.push(3);
  a.pop();
  Queue<int> b = a;
  EXPECT_EQ(b.size(), 2u);
  EXPECT_EQ(b.front(), 2);
  EXPECT_EQ(b.pop(), 2);
  EXPECT_EQ(b.pop(), 3);
  EXPECT_TRUE(b.is_empty());
}

TEST(QueueCopy, IndependenceFromOriginal) {
  Queue<int> a(3);
  a.push(10);
  a.push(20);
  Queue<int> b = a;
  a.push(30);
  EXPECT_EQ(b.size(), 2u);
  EXPECT_EQ(b.front(), 10);
  b.pop();
  EXPECT_EQ(a.front(), 10);
}

TEST(QueueCustomType, Strings) {
  Queue<std::string> q(3);
  q.push("hello");
  q.push("world");
  EXPECT_EQ(q.front(), "hello");
  EXPECT_EQ(q.pop(), "hello");
  EXPECT_EQ(q.pop(), "world");
  EXPECT_TRUE(q.is_empty());
}

TEST(QueueCustomType, CopyStrings) {
  Queue<std::string> a(3);
  a.push("a");
  a.push("b");
  Queue<std::string> b = a;
  EXPECT_EQ(b.size(), 2u);
  EXPECT_EQ(b.pop(), "a");
  EXPECT_EQ(b.pop(), "b");
}

TEST(QueueStress, MixedPushPopPreservesFifo) {
  Queue<int> q(5);
  int nextPush = 0, nextPop = 0;

  for (int i = 0; i < 100000; ++i) {
    if (!q.is_full() && (i % 3 != 0)) {
      q.push(nextPush++);
    } else if (!q.is_empty()) {
      ASSERT_EQ(q.pop(), nextPop++);
    }
  }
  while (!q.is_empty()) ASSERT_EQ(q.pop(), nextPop++);
  EXPECT_EQ(nextPush, nextPop);
}