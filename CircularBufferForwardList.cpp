#include <iostream>
#include <forward_list>
#include <algorithm>
#include <stdexcept>

template <typename T>
class CircularBuffer {
private:			// fields
	size_t counter_ = 0;
	size_t sz_ = 0;
	std::forward_list<T> content_;

	typename std::forward_list<T>::iterator writer_;
	typename std::forward_list<T>::iterator reader_;

public:				// constructors
	CircularBuffer() = delete;
	CircularBuffer(size_t sz);

public:				// methods
	void Set(const T& value) noexcept;
	T Get();
	void Print();

private:			// methods
	void WriterMove();
	void ReaderMove();
	void CounterUp();
	void CounterDown();
};

template<typename T>
CircularBuffer<T>::CircularBuffer(size_t sz) {
	sz_ = sz;
	counter_ = 0;
	for (size_t i = 0; i < sz_; ++i) {
		content_.push_front(T{});
	}

	writer_ = content_.begin()++;
	reader_ = content_.begin();
}

template<typename T>
void CircularBuffer<T>::Set(const T& value) noexcept {
	CounterUp();
	*writer_ = value;
	WriterMove();
}

template<typename T>
T CircularBuffer<T>::Get() {
	CounterDown();		// throw from here, if no more elements to get
	T ret = T{};
	std::swap(ret, *reader_);
	std::cout << "Read value " << ret << std::endl;
	ReaderMove();
	return ret;
}

template<typename T>
void CircularBuffer<T>::Print() {
	std::cout << "Circular buffer size = " << sz_ << ", counter = " << counter_ << std::endl;
	for (const T& i : content_) {
		std::cout << " " << i;
	}
	std::cout << std::endl;
}

template<typename T>
void CircularBuffer<T>::WriterMove() {
	if (writer_ == reader_) {
		ReaderMove();
	}
	if (++writer_ == content_.end()) {
		writer_ = content_.begin();
	}
}

template<typename T>
void CircularBuffer<T>::ReaderMove() {
	if (++reader_ == content_.end()) {
		reader_ = content_.begin();
	}
}

template<typename T>
void CircularBuffer<T>::CounterUp() {
	if (counter_ < sz_) ++counter_;
}

template<typename T>
void CircularBuffer<T>::CounterDown() {
	if (counter_ == 0) {
		std::cout << "Buffer is empty\n";
		throw std::logic_error("Buffer is empty");
	}
	--counter_;
}

int main() {
	CircularBuffer<int> a(7);
	a.Print();
	a.Set(1);
	a.Set(2);
	a.Set(3);
	a.Set(4);
	a.Set(5);
	a.Set(6);
	a.Set(7);
	a.Print();
	a.Set(8);
	a.Set(9);
	a.Set(10);
	a.Print();
	int s = a.Get();
	a.Print();
	s = a.Get();
	s = a.Get();
	s = a.Get();
	s = a.Get();
	s = a.Get();
	a.Print();
}