#include "scan_animation.h"

#include <mrpt/system/datetime.h>
#include <mrpt/maps/CSimplePointsMap.h>

#define MRPT_NO_WARN_BIG_HDR
#include <mrpt/obs.h>

#include <iomanip>

using namespace mrpt;
using namespace mrpt::system;
using namespace mrpt::serialization;
using namespace mrpt::obs;
using namespace mrpt::rtti;

using namespace std;
void BuildMapAndRefresh(CSensoryFrame* , Json::Value& );

void rebuildMaps(int idx, CRawlog& rawlog, Json::Value& ret)
{
  if (idx >= 0 && idx < (int)rawlog.size())
  {
    if (rawlog.getType(idx) == CRawlog::etSensoryFrame)
    {
      CSensoryFrame::Ptr sf = rawlog.getAsObservations(idx);
      BuildMapAndRefresh(sf.get(), ret);
    }
    else if (rawlog.getType(idx) == CRawlog::etObservation)
    {
      CSensoryFrame::Ptr sf = mrpt::make_aligned_shared<CSensoryFrame>();
      sf->insert(rawlog.getAsObservation(idx));
      BuildMapAndRefresh(sf.get(), ret);
    }
  }
}

void BuildMapAndRefresh(CSensoryFrame* sf, Json::Value& ret) {
	// Preprocess: make sure 3D observations are ready:
	std::vector<CObservation3DRangeScan::Ptr> obs3D_to_clear;
	for (CSensoryFrame::iterator it = sf->begin(); it != sf->end(); ++it)
	{
		(*it)->load();
		// force generate 3D point clouds:
		if (IS_CLASS(*it, CObservation3DRangeScan))
		{
			CObservation3DRangeScan::Ptr o =
				std::dynamic_pointer_cast<CObservation3DRangeScan>(*it);
			if (o->hasRangeImage && !o->hasPoints3D)
			{
				mrpt::obs::T3DPointsProjectionParams pp;
				pp.takeIntoAccountSensorPoseOnRobot = false;
				o->project3DPointsFromDepthImageInto(*o, pp);
				obs3D_to_clear.push_back(o);
			}
		}
	}
  /**
   * Not Required
   */
	// // Mix?
	// if (!m_mixlasers)
	// {
	// 	// if not, just clear all old objects:
	// 	for (TListGlObjects::iterator it = m_gl_objects.begin();
	// 		 it != m_gl_objects.end(); ++it)
	// 	{
	// 		TRenderObject& ro = it->second;
	// 		m_plot3D->getOpenGLSceneRef()->removeObject(
	// 			ro.obj);  // Remove from the opengl viewport
	// 	}
	// 	m_gl_objects.clear();
	// }

	// Insert new scans:
	mrpt::system::TTimeStamp tim_last = INVALID_TIMESTAMP;
	bool wereScans = false;
	for (CSensoryFrame::iterator it = sf->begin(); it != sf->end(); ++it)
	{
		const std::string sNameInMap =
			std::string((*it)->GetRuntimeClass()->className) +
			(*it)->sensorLabel;
		if (IS_CLASS(*it, CObservation2DRangeScan))
		{
			CObservation2DRangeScan::Ptr obs =
				std::dynamic_pointer_cast<CObservation2DRangeScan>(*it);
			wereScans = true;
			if (tim_last == INVALID_TIMESTAMP || tim_last < obs->timestamp)
				tim_last = obs->timestamp;
      Json::Value laserScan2D;
      /**
       * Serialize CObservation2DRangeScan obs to Json object
       */
      mrpt::maps::CSimplePointsMap dummMap;
      dummMap.insertionOptions.minDistBetweenLaserPoints = 0;
      dummMap.insertObservation(obs.get());

      vector<float> Xs, Ys;
      dummMap.getAllPoints(Xs, Ys);

      for (int i = 0 ;i < Xs.size(); i++)
      {
        laserScan2D["xs"][i] = Xs[i];

      }

      for (int i = 0;i < Ys.size(); i++)
      {
        laserScan2D["ys"][i] = Ys[i];
      }
      laserScan2D["type"] = "laserScan2D";

      ret[sNameInMap] = laserScan2D;
      /**
       * Add the serialized sensorScan to
       * ret Json object, the key should be the string sNameInMap
       */
			// // Already in the map with the same sensor label?
			// TListGlObjects::iterator it_gl = m_gl_objects.find(sNameInMap);
			// if (it_gl != m_gl_objects.end())
			// {
			// 	// Update existing object:
			// 	TRenderObject& ro = it_gl->second;
			// 	std::dynamic_pointer_cast<CPlanarLaserScan>(ro.obj)->setScan(
			// 		*obs);
			// 	ro.timestamp = obs->timestamp;
			// }
			// else
			// {
			// 	// Create object:
			// 	CPlanarLaserScan::Ptr gl_obj =
			// 		mrpt::make_aligned_shared<CPlanarLaserScan>();
			// 	gl_obj->setScan(*obs);

			// 	TRenderObject ro;
			// 	ro.obj = gl_obj;
			// 	ro.timestamp = obs->timestamp;
			// 	m_gl_objects[sNameInMap] = ro;
			// 	m_plot3D->getOpenGLSceneRef()->insert(gl_obj);
			// }
		}
		else if (IS_CLASS(*it, CObservation3DRangeScan))
		{
      /**
       * TODO: Serialize the CObservationVelodyneScan to be sent
       * to server for 3D visualization
       */
			// CObservation3DRangeScan::Ptr obs =
			// 	std::dynamic_pointer_cast<CObservation3DRangeScan>(*it);
			// wereScans = true;
			// if (tim_last == INVALID_TIMESTAMP || tim_last < obs->timestamp)
			// 	tim_last = obs->timestamp;

			// CColouredPointsMap pointMap;
			// pointMap.colorScheme.scheme =
			// 	CColouredPointsMap::cmFromIntensityImage;
			// pointMap.insertionOptions.minDistBetweenLaserPoints = 0;

			// pointMap.insertObservation(obs.get());

			// // Already in the map with the same sensor label?
			// TListGlObjects::iterator it_gl = m_gl_objects.find(sNameInMap);
			// if (it_gl != m_gl_objects.end())
			// {
			// 	// Update existing object:
			// 	TRenderObject& ro = it_gl->second;
			// 	CPointCloudColoured::Ptr gl_obj =
			// 		std::dynamic_pointer_cast<CPointCloudColoured>(ro.obj);
			// 	gl_obj->loadFromPointsMap(&pointMap);
			// 	ro.timestamp = obs->timestamp;
			// }
			// else
			// {
			// 	// Create object:
			// 	CPointCloudColoured::Ptr gl_obj =
			// 		mrpt::make_aligned_shared<CPointCloudColoured>();
			// 	gl_obj->setPointSize(3.0);
			// 	gl_obj->loadFromPointsMap(&pointMap);

			// 	TRenderObject ro;
			// 	ro.obj = gl_obj;
			// 	ro.timestamp = obs->timestamp;
			// 	m_gl_objects[sNameInMap] = ro;
			// 	m_plot3D->getOpenGLSceneRef()->insert(gl_obj);
			// }
			// // Add to list:
			// //				m_lstScans[obs->sensorLabel] = obs;
		}
		else if (IS_CLASS(*it, CObservationVelodyneScan))
		{
      /**
       * TODO: Serialize the CObservationVelodyneScan to be sent
       * to server for 3D visualization
       */
      // 	CObservationVelodyneScan::Ptr obs =
      // 		std::dynamic_pointer_cast<CObservationVelodyneScan>(*it);
      // 	wereScans = true;
      // 	if (tim_last == INVALID_TIMESTAMP || tim_last < obs->timestamp)
      // 		tim_last = obs->timestamp;

      // 	obs->generatePointCloud();
      // 	CColouredPointsMap pointMap;
      // 	pointMap.loadFromVelodyneScan(*obs);
      // 	obs->point_cloud.clear_deep();

      // 	// Already in the map with the same sensor label?
      // 	TListGlObjects::iterator it_gl = m_gl_objects.find(sNameInMap);
      // 	if (it_gl != m_gl_objects.end())
      // 	{
      // 		// Update existing object:
      // 		TRenderObject& ro = it_gl->second;
      // 		CPointCloudColoured::Ptr gl_obj =
      // 			std::dynamic_pointer_cast<CPointCloudColoured>(ro.obj);
      // 		gl_obj->loadFromPointsMap(&pointMap);
      // 		ro.timestamp = obs->timestamp;
      // 	}
      // 	else
      // 	{
      // 		// Create object:
      // 		CPointCloudColoured::Ptr gl_obj =
      // 			mrpt::make_aligned_shared<CPointCloudColoured>();
      // 		gl_obj->setPointSize(3.0);
      // 		gl_obj->loadFromPointsMap(&pointMap);

      // 		TRenderObject ro;
      // 		ro.obj = gl_obj;
      // 		ro.timestamp = obs->timestamp;
      // 		m_gl_objects[sNameInMap] = ro;
      // 		m_plot3D->getOpenGLSceneRef()->insert(gl_obj);
      // 	}
      // }
	  }
  /**
   * The below code is not needed as
   * there are no old objects in the client side GUI
   */
	// // Check what observations are too old and must be deleted:
	// const double largest_period = 0.2;
	// std::vector<std::string> lst_to_delete;
	// for (auto& o : m_gl_objects)
	// {
	// 	TRenderObject& ro = o.second;

	// 	if ((tim_last == INVALID_TIMESTAMP &&
	// 		 wereScans)  // Scans without timestamps
	// 		|| (tim_last != INVALID_TIMESTAMP &&
	// 			fabs(mrpt::system::timeDifference(ro.timestamp, tim_last)) >
	// 				largest_period))
	// 	{
	// 		lst_to_delete.push_back(o.first);
	// 	}
	// }

	// // Remove too old observations:
	// for (const auto& s : lst_to_delete)
	// {
	// 	TRenderObject& ro = m_gl_objects[s];
	// 	m_plot3D->getOpenGLSceneRef()->removeObject(
	// 		ro.obj);  // Remove from the opengl viewport
	// 	m_gl_objects.erase(s);  // and from my list
	// }

	// // Force refresh view:
	// m_plot3D->setCameraProjective(!cbViewOrtho->IsChecked());
	// m_plot3D->Refresh();

	// // Post-process: unload 3D observations.
	// for (auto& o : *sf) o->unload();
	// for (size_t i = 0; i < obs3D_to_clear.size(); i++)
	// {
	// 	obs3D_to_clear[i]->resizePoints3DVectors(0);
	// 	obs3D_to_clear[i]->hasPoints3D = false;
	// }

  }
}
