/*  Copyright (C) Tim van Mourik, 2014-2016
    Copyright (C) Tim van Mourik, 2017, DCCN
    All rights reserved

 This file is part of the Porcupine pipeline tool, see
 https://github.com/TimVanMourik/Porcupine for the documentation and
 details.

    This toolbox is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This toolbox is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the fmri analysis toolbox. If not, see
    <http://www.gnu.org/licenses/>.
*/

#include <functional>

#include "PythonGenerator.hpp"
#include "Port.hpp"
#include "PortPair.hpp"

PythonGenerator::PythonGenerator() :
    CodeGenerator()
{

}

QString PythonGenerator::generateCode(
            const QList<NodeTreeItem*>& _nodeList,
            const QVector<const Link*>& _linkList
            )
{
    Q_UNUSED(_linkList);
    QString code("#This is a NiPype generator. Warning. Here be dragons.\n");
    code.append("import nipype\n");
    code.append("import nipype.pipeline as pe\n");
    code.append("import nipype.interfaces.fsl as fsl\n");
    code.append("import nipype.interfaces.afni as afni\n");
    code.append("import nipype.interfaces.spm as spm\n");
    code.append("import nipype.interfaces.utility as utility\n\n");

//    code.append("#Where can the raw data be found?\n");
//    code.append("grabber = nipype.DataGrabber()\n");
//    code.append("grabber.inputs.base_directory = experiment_dir + '/data'\n");
//    code.append("grabber.inputs.subject_id = nameofsubjects\n\n");

//    code.append("#Where should the output data be stored at?\n");
//    code.append("sink = nipype.DataSink()\n");
//    code.append("sink.inputs.base_directory = experiment_dir + '/output_folder'\n\n");

    foreach(const NodeTreeItem* item, _nodeList)
    {
        code.append(itemToCode(item));
    }
    code.append("#Create a workflow to connect all those nodes\n");
    code.append("analysisflow = nipype.Workflow('MyWorkflow')\n");

    foreach(const Link* link, _linkList)
    {
        code.append(linkToCode(link));
    }
//    link.setAttribute("from", QString::number((quint64) m_portFrom, 16));
//    link.setAttribute("to", QString::number((quint64) m_portTo, 16));


    code.append("\n#Run the workflow in parallel\n");
    code.append("analysisflow.run()\n");

    return code;
}

QString PythonGenerator::itemToCode(
        const NodeTreeItem* _item
        ) const
{
    const NodeSetting* nodeSetting = _item->getNodeSetting();
    QString code("");
    if(nodeSetting->getTitle().getArgument("NiPype").isEmpty())
    {
        return QString("");
    }

    QString nodeName = QString("NodeHash_%1").arg(QString::number((quint64) _item->getNode(), 16));
    code.append(QString("#%1\n").arg(nodeSetting->getTitle().getComment("NiPype")));
    code.append(QString("%1 = pe.Node(interface = %2, ").arg(nodeName, nodeSetting->getTitle().getArgument("NiPype")));
    code.append(QString("name = 'NodeName_%1')\n").arg(QString::number((quint64) _item->getNode(), 16)));
    foreach (Argument argument, nodeSetting->getPorts())
    {
        QString filename = _item->getFileName(argument.getName());
        if(!filename.isEmpty())
        {
            if(argument.getType() == Argument::INPUT || argument.getType() == Argument::INOUT || argument.getType() == Argument::HIDDEN || argument.getType() == Argument::SECRET)
            {
                code.append(QString("%1.inputs.%2 = %3\n").arg(nodeName, argument.getArgument("NiPype"), filename));
            }
        }
    }

    code.append("\n");
    return code;
}

QString PythonGenerator::linkToCode(
        const Link* _link
        ) const
{
    QString code("");
    QString source =  QString("NodeHash_%1").arg(QString::number((quint64) _link->getPortFrom()->getNode(), 16));
    QString destination =  QString("NodeHash_%1").arg(QString::number((quint64) _link->getPortTo()->getNode(), 16));
    QString sourceAttribute(_link->getPortFrom()->getPortPair()->getArgument()->getArgument("NiPype"));
    QString destinationAttribute(_link->getPortTo()->getPortPair()->getArgument()->getArgument("NiPype"));
    code.append(QString("analysisflow.connect(%1, '%2', %3, '%4')\n").arg(source, sourceAttribute, destination, destinationAttribute));

    return code;
}

PythonGenerator::~PythonGenerator()
{

}

