#ifndef MATERIAL_AI_H__
#define MATERIAL_AI_H__

#include "minmax_ai.h"
#include "../core/position.h"

namespace ai {

class MaterialAI : public MinMaxAI {
 public:
  MaterialAI();
  virtual ~MaterialAI();
  
  virtual int Evaluate(core::Position* position);
};


}

#endif
