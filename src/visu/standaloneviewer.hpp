#include <QApplication>
#include <QMainWindow>

#include "kgd/external/cxxopts.hpp"
#include "kgd/external/json.hpp"
#include "kgd/utils/utils.h"

#include "kgd/settings/configfile.h"

#include "phylogenyviewer.h"
#include "speciestracking.h"


/// TODO REMOVE
#include <QDebug>


template <typename GENOME, typename UDATA>
int run(int argc, char *argv[]) {
  using PTree = phylogeny::PhylogeneticTree<GENOME, UDATA>;
  using PViewer = gui::PhylogenyViewer<GENOME, UDATA>;
  using Verbosity = config::Verbosity;

  auto config = PViewer::defaultConfig();

  std::string configFile, ptreeFile;
  Verbosity verbosity = Verbosity::SHOW;
  std::string outfile;

  static const auto dirFromStr = [] {
    using D = QBoxLayout::Direction;
    QMap<QString, D> map {
      { "LR", D::LeftToRight }, { "TB", D::TopToBottom },
      { "RL", D::RightToLeft }, { "BT", D::BottomToTop }
    };
    return map;
  }();
  std::string layoutStr = "LR";

  std::string customColors;

  cxxopts::Options options("PTreeViewer", "Loads and displays a phenotypic tree"
                           " for \"" + utils::className<GENOME>()
                           + "\" genomes");
  options.add_options()
    ("h,help", "Display help")
    ("c,config", "File containing configuration data",
     cxxopts::value(configFile))
    ("v,verbosity", "Verbosity level. " + config::verbosityValues(),
     cxxopts::value(verbosity))
    ("t,tree", "File containing the phenotypic tree [MANDATORY]",
     cxxopts::value(ptreeFile))
    ("min-survival", "Minimal survival duration",
     cxxopts::value(config.minSurvival))
    ("min-enveloppe", "Minimal fullness for the enveloppe",
     cxxopts::value(config.minEnveloppe))
    ("survivors-only", "Whether or not to only show paths leading to still-alive"
                      " species",
     cxxopts::value(config.survivorsOnly)->default_value("false"))
    ("show-names", "Whether or not to show node names",
     cxxopts::value(config.showNames)->default_value("true"))
    ("p,print", "Render p-tree into 'filename'", cxxopts::value(outfile))
    ("radius", "Tree rendering radius", cxxopts::value(config.rasterRadius))
    ("layout", "Layout for the graph/controls. Valid values are "
               + QStringList(dirFromStr.keys()).join(", ").toStdString(),
     cxxopts::value(layoutStr))
    ("colors", "Custom colors for species tracking ID1:Color1 ID2:Color2 ...",
     cxxopts::value(customColors))
    ;

  auto result = options.parse(argc, argv);
  options.parse_positional("tree");

  if (result.count("help")) {
      std::cout << options.help() << std::endl;
      return 0;
  }

  if (result.count("tree") != 1) {
    std::cerr << "Missing mandatory argument 'tree'" << std::endl;
    return 1;
  }

  config::PViewer::setupConfig(configFile, verbosity);

  if (!result.count("show-names"))
    config.showNames = config::PViewer::showNodeNames();
  if (!result.count("min-survival"))
    config.minSurvival = config::PViewer::minNodeSurvival();
  if (!result.count("min-enveloppe"))
    config.minEnveloppe = config::PViewer::minNodeEnveloppe();
  if (!result.count("survivors-only"))
    config.survivorsOnly = config::PViewer::survivorNodesOnly();

  if (!customColors.empty()) {
    config.color = gui::ViewerConfig::CUSTOM;
    for (const std::string sspec: utils::split(customColors, ' ')) {
      std::stringstream ss (sspec);
      char c;
      uint sid, cid;
      std::string scolor;

      ss >> sid >> c >> scolor;
      QColor color;
      if (std::istringstream (scolor) >> cid)
        color = gui::species_tracking::ColorDelegate::nextColor(cid);
      else
        color = QColor(QString::fromStdString(scolor));

      if (ss)
        config.colorSpecs.insert({phylogeny::SID(sid), color, true});
      else
        std::cerr << "Failed to parse color spec '" << sspec << "'. Ignoring..."
                  << std::endl;
    }
    std::cout << "Parsed custom colors:\n";
    for (const auto &spec: config.colorSpecs)
      std::cout << "{" << spec.sid << " {"
                  << spec.color.red() << ","
                  << spec.color.green() << ","
                  << spec.color.blue()
                << "}, " << spec.enabled << "}\n";
    std::cout << std::endl;
  }

  QApplication a(argc, argv);
  setlocale(LC_NUMERIC,"C");

  PTree pt = PTree::readFrom(ptreeFile);

  auto layoutDir = dirFromStr.value(QString::fromStdString(layoutStr));
  PViewer pv (nullptr, pt, layoutDir, config);

  if (outfile.empty()) {
    pv.show();
    pv.setMinimumSize(500, 500);
    return a.exec();

  } else {
    pv.renderTo(QString::fromStdString(outfile));
    return 0;
  }
}
