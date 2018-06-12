// OpenClosedPrinciples.cpp : Defines the entry point for the console application.
//

#include "stdafx.h";
#include <iostream>
#include <string>
#include <vector>
using namespace std;

enum class Color
{
	Red,
	Green,
	Blue
};

enum class Size
{
	Small,
	Median,
	Large
};

struct Product
{
	string name;
	Color color;
	Size size;
};

struct ProductFilter
{
	static vector<Product*> filter_by_color(vector<Product*> products, Color color)
	{
		vector<Product*> result;
		for (auto& product : products)
		{
			if (color == product->color)
			{
				result.push_back(product);
			}
		}
		return result;
	}
};

template <typename T> struct Specification
{
	virtual ~Specification() = default;
	virtual bool is_satisfied(T* item) = 0;
};

template <typename T> struct Filter
{
	virtual vector<T*> filter(vector<T*> items, Specification<T>& specification) = 0;
};

struct BetterFilter:Filter<Product>
{
	vector<Product*> filter(vector<Product*> items, Specification<Product>& specification) override
	{
		vector<Product*> result;
		for (Product* item : items)
		{
			if(specification.is_satisfied(item))
			{
				result.push_back(item);
			}
		}
		return result;
	}
};

struct ColorSpecification:Specification<Product>
{
	Color color;
	explicit ColorSpecification(Color color) :color(color) {};
	bool is_satisfied(Product* item) override
	{
		return item->color == color;
	}
};

int main()
{
	Product apple{ "Apple",Color::Red,Size::Small };
	Product tree{ "tree",Color::Green,Size::Median };
	Product house{"house",Color::Green,Size::Large };
	vector<Product*> products = { &apple,&tree,&house };
	BetterFilter better_filter;
	ColorSpecification color_specification(Color::Green);
	auto result = better_filter.filter(products, color_specification);
	for(auto& i:result)
	{
		cout << i->name << " ";
	}
	return 0;
}
