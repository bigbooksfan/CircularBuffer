#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

template <typename T>
class CircularBuffer {
private:			// fields
	size_t counter_ = 0;
	std::vector<T> content_;

	typename std::vector<T>::iterator writer_;
	typename std::vector<T>::iterator reader_;

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
	content_.resize(sz);
	content_.shrink_to_fit();		// if capacity somewhy greater than size
	counter_ = 0;

	writer_ = content_.begin();
	reader_ = content_.end() - 1;
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
	std::cout << "Circular buffer size = " << content_.size() << ", counter = " << counter_ << std::endl;
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
	if (counter_ < content_.size()) ++counter_;
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