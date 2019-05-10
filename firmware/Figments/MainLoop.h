#pragma once

#include <vector>
#include <algorithm>
#include "./Input.h"

class Task;
class InputSource;

struct Scheduler {
    std::vector<Task*> tasks;

    bool operator==(const Scheduler& other) const {
        return tasks == other.tasks;
    }

    Scheduler(std::vector<Task*> &&tasks)
      : tasks(std::move(tasks))
      {}

    struct iterator: public std::iterator<std::input_iterator_tag, Task*> {
        Scheduler& queue;
        int idx = 0;
        explicit iterator(Scheduler& queue) : queue(queue), idx(nextEnabled(0)) {}
        iterator(Scheduler& queue, int start) : queue(queue), idx(nextEnabled(start)) {}
        iterator& operator++() {
            while (idx < queue.tasks.size() && !queue.tasks[idx]->state == Task::Running) {
                idx++;
            }
            idx = nextEnabled(idx+1);
            return *this;
        }

        int nextEnabled(int start) const {
            for(int pos = start; pos < queue.tasks.size();pos++) {
                if (queue.tasks[pos]->state == Task::Running) {
                    return pos;
                }
            }
            return queue.tasks.size();
        }

        iterator operator++(int) {iterator ret = *this; ++(*this); return ret;}
        bool operator==(iterator other) const { return idx == other.idx && queue == other.queue; }
        bool operator!=(iterator other) const { return !(*this == other); }
        Task* operator*() const { return queue.tasks[idx]; }
    };

    iterator begin() { return iterator(*this); }
    iterator end() { return iterator(*this, tasks.size()); }
};

template<typename T, int Size>
struct Ringbuf {
    Ringbuf() : m_head(0), m_tail(0) {}

    void clear() {
        m_head = 0;
        m_tail = 0;
    }

    bool take(T& dest) {
        if (m_head == m_tail) {
            return false;
        }
        const int cur = m_head;
        const int nextHead = (m_head + 1) % Size;
        m_head = nextHead;
        dest = m_items[cur];
        return true;
    }

    void insert(const T& src) {
        const int cur = m_tail;
        const int nextTail = (m_tail + 1) % Size;
        if (nextTail == m_head) {
            return;
        } else {
            m_tail = nextTail;
        }
        m_items[cur] = src;
    }
private:
    int m_head = 0;
    int m_tail = 0;
    std::array<T, Size> m_items;
};


struct MainLoop {
    Scheduler scheduler;

    MainLoop(std::vector<Task*> &&tasks)
        : scheduler(std::move(tasks)) {s_instance = this;}

    void start();
    void loop();
    void dispatch(const InputEvent& event);
    static MainLoop* instance() { return s_instance; }

private:
    Ringbuf<InputEvent, 10> m_eventBuf;

    static MainLoop* s_instance;
};
