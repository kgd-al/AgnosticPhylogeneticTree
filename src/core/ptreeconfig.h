#ifndef KGD_P_TREE_CONFIG_H
#define KGD_P_TREE_CONFIG_H

#include "kgd/settings/configfile.h"

/*!
 * \file ptreeconfig.h
 *
 * Definitions for controlling the phylogenic process
 */

namespace config {

#define CFILE PTree

/// Config file for the phylogenic algorithms
struct CFILE : public ConfigFile<CFILE> {

  /// Threshold for being considered a "viable mate"
  DECLARE_PARAMETER(double, compatibilityThreshold)

  /// Number of votes required to being considered as belonging
  DECLARE_PARAMETER(double, similarityThreshold)

  /// Average compatibility required to being considered as member of species
  DECLARE_PARAMETER(double, avgCompatibilityThreshold)

  /// Number of genome stored as enveloppe points
  DECLARE_PARAMETER(uint, rsetSize)

  /// Whether to put extra effort in creating new species or plain singletons
  DECLARE_PARAMETER(bool, simpleNewSpecies)

  /// How often to perform the stillborn garbage collection
  DECLARE_PARAMETER(uint, stillbornTrimmingPeriod)

  /// How much of the enveloppe should be filled to count as a regular species
  DECLARE_PARAMETER(float, stillbornTrimmingThreshold)

  /// How long to wait for a stillborn to gain new individuals
  DECLARE_PARAMETER(float, stillbornTrimmingDelay)

  /// How long to wait for before considering trimming a species
  DECLARE_PARAMETER(uint, stillbornTrimmingMinDelay)

  /// (Debug) selector for the species matching score computing type
  DECLARE_DEBUG_PARAMETER(bool, DEBUG_FULL_CONTINUOUS, true)

  /// (Debug) selector for the enveloppe criteria
  DECLARE_DEBUG_PARAMETER(int, DEBUG_ENV_CRIT, 1)

  /// How much debug information should be printed out
  DECLARE_DEBUG_PARAMETER(uint, DEBUG_LEVEL, 0)

  /// Should debug info about the ptree be printed out ?
  DECLARE_DEBUG_PARAMETER(bool, DEBUG_PTREE, 0)

  /// Should debug info about the enveloppes be printed out ?
  DECLARE_DEBUG_PARAMETER(bool, DEBUG_ENVELOPPE, 0)

  /// Should debug info about the contributors be printed out ?
  DECLARE_DEBUG_PARAMETER(bool, DEBUG_CONTRIBUTORS, 0)

  /// Should debug info about the idToSpecies map be printed out ?
  DECLARE_DEBUG_PARAMETER(bool, DEBUG_ID2SPECIES, 0)

  /// Should debug info about the stillborn trimming be printed out ?
  DECLARE_DEBUG_PARAMETER(bool, DEBUG_STILLBORNS, 0)
};
#undef CFILE

} // end of namespace config

#endif // KGD_P_TREE_CONFIG_H
