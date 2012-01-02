#define FISH_HPP
#ifdef FISH_HPP

#include <iostream>
#include <cstdlib>
#include <cstdio>

class Fish {
 public:
  enum FeedType { CARNIVORE, HERBIVORE };

  int speed() {
    return speed_;
  }
  int weight() {
    return weight_;
  }
  int age() {
    return age_;
  }

 private:
  std::string species_name_;
  FeedType feed_type_;
  int speed_;
  int weight_;
  int min_weight_;
  int max_weight_;
  int age_;
  int attack_damage_;

}; // Fish

#endif // FISH_HPP
