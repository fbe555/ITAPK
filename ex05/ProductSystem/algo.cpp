/*****************************************/
/* Copyright: DevelEdu 2013              */
/* Author: sha                           */
/*****************************************/

#include <fstream>
#include <iostream>
#include <iterator>

#include <algorithm>
#include <numeric>
#include <functional>
#include <vector>
#include <string>

class Product {
public:
    Product(const std::string &name, float price, unsigned int sold = 0)
            : name_(name), price_(price), sold_(sold) {}

    Product()
            : name_(""), price_(0), sold_(0) {}

    const std::string &name() const {
        return name_;
    }

    float price() const {
        return price_;
    }

    void setPrice(float newPrice) {
        price_ = newPrice;
    }

    unsigned int sold() const {
        return sold_;
    }

    void setDiscount(float discountPercentage) {
        price_ = price_*(1-(discountPercentage/100));
    }

    friend std::istream &operator>>(std::istream &i, Product &p);

    friend std::ostream &operator<<(std::ostream &o, const Product &p);

private:
    std::string name_;
    float price_;
    int sold_;
};

typedef std::vector<Product> ProductList;


std::ostream &operator<<(std::ostream &o, const Product &p) {
    return o << p.name_ << " " << p.price_ << " " << p.sold_;
}

std::istream &operator>>(std::istream &i, Product &p) {
    return i >> p.name_ >> p.price_ >> p.sold_;
}

/**
 * Read products from file
 */
bool productDBRead(ProductList& pl, const std::string& fileName)
{
    pl.clear();
    std::ifstream pFile(fileName.c_str());
    if(!pFile.is_open()) return false;
    std::istream_iterator<Product> begin_it(pFile);
    std::istream_iterator<Product> end_it;
    std::copy(begin_it, end_it, std::back_inserter(pl));
    return true;
}


/**
 * Printout all products
 */
void printAll(const ProductList &pl) {
    std::cout << "##################################################" << std::endl;
    std::cout << "Printing out all products..." << std::endl;
    std::cout << "----------------------------" << std::endl;
    std::ostream_iterator<Product> out_it(std::cout, "\n\r");
    copy(pl.begin(), pl.end(), out_it);
//    for(ProductList::const_iterator iter = pl.begin(); iter != pl.end(); ++iter)
//    {
//        std::cout << *iter << std::endl;
//    }
    std::cout << "##################################################" << std::endl;
}


/**
   Add item
*/
void addItem(ProductList &pl) {
    std::string productName;
    float productPrice;
    std::cout << "Please enter the product name: ";
    std::cin >> productName;
    std::cout << std::endl << "Please enter the product price: ";
    std::cin >> productPrice;
    pl.push_back(Product(productName, productPrice));
    std::cout << "Product added." << std::endl;
}


/**
   Write data to db file
*/
void productDBWrite(const ProductList &pl, const std::string &fileName) {
    std::ofstream pFile(fileName.c_str());
    std::copy(pl.begin(), pl.end(), std::ostream_iterator<Product>(pFile, "\n"));
}

struct salesComp {
    salesComp(const int sales) : _sales(sales) {}

    bool operator()(const Product &p) {
        return p.sold() > _sales;
    }

    int _sales;
};

//bool checkNumberOfSales(const int _sales, const Product& product)
//{
//    return product.sold() < _sales;
//}

/**
 * Print poorly selling products
 */
void printPoorlySellingProducts(const ProductList &pl) {
    std::ostream_iterator<Product> out_it(std::cout, "\n\r");

    std::remove_copy_if(pl.begin(),
                        pl.end(),
                        out_it,
                        salesComp(10));
}

/***********************************************************
 *                    Discount functor                     *
 ***********************************************************/
struct DiscountFunctor {
    DiscountFunctor(float percentDiscount) : _dis((1 - (percentDiscount / 100))) {}

    void operator()(Product &p) {
        p.setPrice(p.price() * _dis);
    }

    float _dis;
};

struct DiscountTransFunctor {
    DiscountTransFunctor(float percentDiscount) : _dis((1 - (percentDiscount / 100))) {}

    Product operator()(Product &p) {
        Product new_p = Product(p);
        new_p.setPrice(p.price() * _dis);
        return new_p;

    }

    float _dis;
};

/**
 * Set a discount on all products - Using for_each()
 */
void addDiscountUsingForEach(ProductList &pl, float percentDiscount) {
    std::for_each(pl.begin(), pl.end(), DiscountFunctor(percentDiscount));
}


/**
 * Set a discount on all products - Using transform()
 */
void addDiscountUsingTransform(ProductList &pl, float percentDiscout) {
    std::cout << "#########################################################" << std::endl
              << "Printing discounted product list: " << std::endl;
    std::transform(pl.begin(), pl.end(), std::ostream_iterator<Product>(std::cout, "\n\r"),
                   DiscountTransFunctor(percentDiscout));
    std::cout << "Please note this discount is not applied to the actual product list!" << std::endl
              << "#########################################################" << std::endl;
}
void addDiscountUsingLambda(ProductList &pl, float percentDiscount) {
    std::for_each(pl.begin(), pl.end(),
                  [percentDiscount](Product &p){
                      p.setDiscount(percentDiscount);
                  });
}

/**
 * Calculate the total amount of sold products
 */
void calcTotalSoldProducts(ProductList &pl) {
    std::vector<unsigned int> sold_product_count;
    std::cout << "########################################################" << std::endl
              << "Printing total amount of products sold" << std::endl;
    std::transform(pl.begin(), pl.end(), std::back_inserter(sold_product_count), std::mem_fun_ref(&Product::sold)); /////sold_product_count);
    int accum = std::accumulate(sold_product_count.begin(),
                    sold_product_count.end(), 0);
    std::cout << accum << std::endl;
    std::cout << "########################################################" << std::endl;
}


/**
 * Setting discount using bind2nd - OPTIONAL
 */