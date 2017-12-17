#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <json.hpp>

#include "common.h"

using JSON = nlohmann::json;

/**
 * Menu items
 */
struct MenuItem {
  std::string item;
  std::string description;
  float price;
  int id;
};

/**
 * Menu, containing list of items that can
 * be ordered at the restaurant
 */
class Menu {
  std::map<int,MenuItem> menu_;
  std::vector<MenuItem> catalogue_;

 public:
  Menu(): menu_(), catalogue_(){}

  /**
   * Loads items from a JSON file
   * @param filename file to load items from
   * @return number of new items added to the menu
   */
  size_t load(const std::string& filename) {
    std::ifstream fin(filename);
    size_t numberAdded = 0;
    size_t numberOfItems = 25; // I chose this arbitrarily so feel free to adjust
    cpen333::process::shared_object<InventoryData> inventory(INVENTORY_MEMORY_NAME);

    if (fin.is_open()) {
      JSON jmenu;
      fin >> jmenu;

      for (const auto& jitem : jmenu) {

        //Writes to the menu struct so users can later access it
        MenuItem item;
        item.item = jitem["item"];
        item.description = jitem["description"];
        item.price = (float)jitem["price"];
        item.id = jitem["item_id"];

        //Writes to the inventory struct so robots can later access it
        inventory->stock[numberAdded][ITEM_ID_INDEX] = jitem["item_id"];
        inventory->stock[numberAdded][ITEM_QUANTITY_INDEX] = numberOfItems; 

        // add new item
        auto it = menu_.insert({item.id, item});
        if (it.second) {
          ++numberAdded;
          catalogue_.push_back(item);
        }
      }
    }
    return numberAdded;
  }

  /**
   * Catalogue
   * @return list of the catalogue
   */
  const std::vector<MenuItem>& catalogue() const {
    return catalogue_;
  }

};

#endif //MENU_H
