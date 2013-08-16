#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

#include "sim.h"
#include <algorithm>
#include <cmath>
#include "rules.h"
#include "tools.h"
#include "init.h"
#include "prepare.h"
#include "utils/MapTools.h"
#include "utils/computeAddressSimilarity.h"
#include "utils/classify_data/classify_data.h"
#include "utils/classify_data/string_process.h"


class GenDistanceDealRule : public ResourceRule<GenDistanceDealRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	sim_temp_cache->m_distance = MapTools::get_distance(base_data->m_lat,
            			base_data->m_lng, 
                        inc_data->m_lat, inc_data->m_lng);
            return false;
                
    }
};

REGINTER_RULE( GenDistanceDeal )