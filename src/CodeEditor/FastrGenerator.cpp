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

#include "FastrGenerator.hpp"
#include "Port.hpp"
#include "PortPair.hpp"

FastrGenerator::FastrGenerator() :
    CodeGenerator()
{

}

QString FastrGenerator::generateCode(
            const QList<NodeTreeItem*>& _nodeList,
            const QVector<const Link*>& _linkList
            )
{
    Q_UNUSED(_linkList);
    QString code("#This is a Fastr generator. Warning. Here be cute puppies.\n");
    code.append("import fastr\n");
    code.append("network = fastr.create_network()\n");

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

    foreach(const Link* link, _linkList)
    {
        code.append(linkToCode(link));
    }
//    link.setAttribute("from", QString::number((quint64) m_portFrom, 16));
//    link.setAttribute("to", QString::number((quint64) m_portTo, 16));


    code.append("\n# Draw the network\n");
    code.append("network.draw_network(draw_dimension=True)\n");

    return code;
}

QString FastrGenerator::itemToCode(
        const NodeTreeItem* _item
        ) const
{
    const NodeSetting* nodeSetting = _item->getNodeSetting();
    QString code("");
    if(nodeSetting->getTitle().getArgument("fastr").isEmpty())
    {
        return QString("");
    }

    QString nodeName = QString("NodeHash_%1").arg(QString::number((quint64) _item->getNode(), 16));
    code.append(QString("#%1\n").arg(nodeSetting->getTitle().getComment("fastr")));
    code.append(QString("%1 = network.create_node(%2, id_=%1").arg(nodeName, nodeSetting->getTitle().getArgument("fastr")));
    code.append(QString("name = 'NodeName_%1')\n").arg(QString::number((quint64) _item->getNode(), 16)));
    foreach (Argument argument, nodeSetting->getPorts())
    {
        QString filename = _item->getFileName(argument.getName());
        if(!filename.isEmpty())
        {
            if(argument.getType() == Argument::INPUT || argument.getType() == Argument::INOUT || argument.getType() == Argument::HIDDEN || argument.getType() == Argument::SECRET)
            {
                code.append(QString("%1.inputs.%2 = %3\n").arg(nodeName, argument.getArgument("fastr"), filename));
            }
        }
    }

    code.append("\n");
    return code;
}

QString FastrGenerator::linkToCode(
        const Link* _link
        ) const
{
    QString code("");
    QString source =  QString("NodeHash_%1").arg(QString::number((quint64) _link->getPortFrom()->getNode(), 16));
    QString destination =  QString("NodeHash_%1").arg(QString::number((quint64) _link->getPortTo()->getNode(), 16));
    QString sourceAttribute(_link->getPortFrom()->getPortPair()->getArgument()->getArgument("fastr"));
    QString destinationAttribute(_link->getPortTo()->getPortPair()->getArgument()->getArgument("fastr"));
    code.append(QString("analysisflow.connect(%1, '%2', %3, '%4')\n").arg(source, sourceAttribute, destination, destinationAttribute));

    return code;
}

FastrGenerator::~FastrGenerator()
{

}

