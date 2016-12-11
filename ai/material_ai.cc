#include "material_ai.h"
#include "ai_util.h"

#include <cstdlib>
#include <ctime>

namespace ai {

MaterialAI::MaterialAI() : MinMaxAI() {
  srand(time(NULL));
}

MaterialAI::~MaterialAI() {}

int MaterialAI::Evaluate(core::Position* position) {
  int material_difference = MaterialDifference(*position, 100, 300, 300, 500, 900);
  int random_component = RandomInt(-10, 10);
  return material_difference + random_component;
}

}
