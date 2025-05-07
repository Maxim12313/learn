#include <atomic>
#include <condition_variable>
#include <fcntl.h>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <termios.h>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std;

void set_raw_mode(bool enable) {
    static struct termios oldt, newt;
    if (enable) {
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO); // Turn off line buffering and echo
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
}

void set_nonblocking(bool enable) {
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    if (enable)
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    else
        fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);
}

enum EventType {
    KEYPRESS,
    QUIT,
};

struct Event {
    char key;
    EventType type;
};

char wait_input() {
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    select(STDIN_FILENO + 1, &fds, NULL, NULL, NULL);

    char c;
    read(STDIN_FILENO, &c, 1);
    return c;
}

atomic<bool> running;
queue<Event> events;
mutex events_mtx;

void get_events() {
    while (running) {
        char c = wait_input();
        lock_guard<mutex> lck(events_mtx);
        if (events.size() < 4)
            events.push(Event{c, KEYPRESS});
    }
}

// blocks until at least 1 event
vector<Event> receive_events() {
    unique_lock<mutex> lck(events_mtx);
    if (!running || events.empty())
        return {};

    vector<Event> to_process(events.size());
    for (int i = 0; i < to_process.size(); i++) {
        to_process[i] = std::move(events.front());
        events.pop();
    }

    return to_process;
}

int handle_a() {
    cout << "started\n";
    for (int i = 0; i < 1e9; i++) {
    }
    cout << "finished\n";
    return 5;
}

void process_event(Event &e) {
    if (e.key == 'Q') {
        running.store(false);
    } else if (e.key == 'a') {
        thread t1(handle_a);
        t1.detach();
    }
}

void reset_screen() {
    cout << "\033[2J"; // Clear entire screen
    cout << "\033[H";  // Move cursor to home}
}

void draw_cursor(int row, int col) {
    cout << "\033[" << row << ";" << col << "H";
}

int main() {
    set_raw_mode(true);
    set_nonblocking(true);
    running.store(true);
    thread event_handler(get_events);
    while (running) {
        vector<Event> to_process = receive_events();
        for (Event &e : to_process)
            process_event(e);
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    event_handler.join();
}
