#include "material_ai.h"
#include "ai_util.h"

#include <cstdlib>
#include <ctime>

namespace ai {

MaterialAI::MaterialAI() : MinMaxAI() {
  srand(time(NULL));
}

MaterialAI::~MaterialAI() {}

double MaterialAI::Evaluate(core::Position position) {
  double material_difference = MaterialDifference(position, 1.0, 3.0, 3.0, 5.0, 9.0);
  double random_component = RandomDouble(-0.1, 0.1);
  return material_difference + random_component;
}

}
