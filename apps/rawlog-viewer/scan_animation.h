#pragma once
#include "CRawlogTreeProcessor.h"
#include <mrpt/obs/CSensoryFrame.h>
void rebuildMaps(int idx, mrpt::obs::CRawlog& rawlog, Json::Value& ret);
