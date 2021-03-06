#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

enum class Color
{
	red,
	green,
	blue
};

enum class Size
{
	small,
	medium,
	large
};

struct product
{
	string name;
	Color color;
	Size size;
};

struct ProductFilter
{
	vector<product*> by_color(vector<product*> items, Color color)
	{
		vector<product*> result;
		for (auto& i : items)
		{
			if (i->color == color)
				result.push_back(i);
		}
		return result;
	}

	vector<product*> by_size(vector<product*> items, Size size)
	{
		vector<product*> result;
		for (auto& i : items)
		{
			if (i->size == size)
				result.push_back(i);
		}
		return result;
	}

	vector<product*> by_color_and_size(vector<product*> items, Color color, Size size)
	{
		vector<product*> result;
		for (auto& i : items)
		{
			if (i->color == color && i->size == size)
				result.push_back(i);
		}
		return result;
	}
};

template <typename T> struct AndSpecification;

template <typename T> struct Specification
{
	virtual bool is_satisfied(T* item) = 0;

	AndSpecification<T> operator&&(Specification<T>&& other)
	{
		return AndSpecification<T>(*this, other );
	}
};

template <typename T>
struct Filter
{
	virtual vector<T*> filter(vector<T*> items, Specification<T>& spec) = 0;
};

struct BetterFilter : Filter<product>
{
	vector<product*> filter(vector<product*> items, Specification<product>& spec) override
	{
		vector<product*> result;
		for (auto& item : items)
		{
			if (spec.is_satisfied(item))
				result.push_back(item);
		}
		return result;
	}
};

struct ColorSpecification : Specification<product>
{
	Color color;

	explicit ColorSpecification(Color color)
		: color(color)
	{
	}

	bool is_satisfied(product* item) override
	{
		return item->color == color;
	}
};

struct SizeSpecification:Specification<product>
{
	Size size;

	explicit SizeSpecification(Size size)
		: size(size)
	{
	}

	bool is_satisfied(product* item) override
	{
		return item->size == size;
	}
};

// combinator
template <typename T> struct AndSpecification:Specification<T>
{
	Specification<T>& first;
	Specification<T>& second;

	AndSpecification(Specification<T>& first, Specification<T>& second)
		: first(first),
		  second(second)
	{
	}

	bool is_satisfied(T* item) override
	{
		return first.is_satisfied(item) && second.is_satisfied(item);
	}
};


int main()
{
	product apple{"apple", Color::green, Size::small};
	product tree{"tree", Color::green, Size::large};
	product house{"house", Color::blue, Size::large};

	vector<product*> items{&apple, &tree, &house};
	BetterFilter bf;
	// ColorSpecification color_specification(Color::green);
	// SizeSpecification size_specification(Size::large);
	// AndSpecification<product> and_specification(color_specification, size_specification);
	auto specs = ColorSpecification(Color::green) && SizeSpecification(Size::large);
	vector<product*> result = bf.filter(items, specs);
	for (product* item : result)
	{
		cout << item->name << " ";
	}
}
