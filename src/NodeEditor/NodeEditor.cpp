/*
 * Copyright (C) 2014 Tim van Mourik
*/

#include <iostream>

#include <QDomDocument>
#include <QDomElement>
#include <QPointF>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

#include "Link.hpp"
#include "Node.hpp"
#include "NodeEditor.hpp"
#include "NodeTreeEditor.hpp"
#include "Port.hpp"
#include "Preferences.hpp"

NodeEditor::NodeEditor(
        QWidget* _parent
        ) :
    QGraphicsView(_parent),
    m_newLink(0),
    m_treeModel(0)
{
}

void NodeEditor::install(
        )
{
    Preferences& preferences = Preferences::getInstance();
    QGraphicsScene* scene = new QGraphicsScene();
    scene->setBackgroundBrush(preferences.getSceneBackgroundBrush());
    setScene(scene);

    //makes sure that all events from the scene are passed on to the editor
    scene->installEventFilter(this);
    show();
}

QGraphicsScene* NodeEditor::getScene(
        ) const
{
    return scene();
}

bool NodeEditor::eventFilter(
        QObject* _object,
        QEvent* _event)
{
    QGraphicsSceneMouseEvent* mouseEvent = (QGraphicsSceneMouseEvent*) _event;

    switch((int) _event->type())
    {
    case QEvent::GraphicsSceneMousePress:
    {
        switch ((int) mouseEvent->button())
        {
        case Qt::LeftButton:
        {
            QGraphicsItem* item = itemAt(mouseEvent->scenePos());
            if (item && item->type() == Port::Type)
            {
                m_newLink = new Link(scene());

                Port* port = (Port*) item;
                if(port->getPortType() == Port::PortType::INPUT)
                {
                    m_newLink->setPortTo((Port*) item);
                }
                else if(port->getPortType() == Port::PortType::OUTPUT)
                {
                    m_newLink->setPortFrom((Port*) item);
                }
                else
                {
                    delete m_newLink;
                    break;
                }
                m_newLink->setPositionFrom(item->scenePos());
                m_newLink->setPositionTo(mouseEvent->scenePos());
                m_newLink->updatePath();
                return true;
            }
            break;
        }
        }
    }
    case QEvent::GraphicsSceneMouseMove:
    {
        if (m_newLink)
        {
            m_newLink->setPositionTo(mouseEvent->scenePos());
            m_newLink->updatePath();
            return true;
        }
        break;
    }
    case QEvent::GraphicsSceneMouseRelease:
    {
        if (m_newLink && mouseEvent->button() == Qt::LeftButton)
        {
            QGraphicsItem* item = itemAt(mouseEvent->scenePos());
            if (item && item->type() == Port::Type)
            {
                Port* portCreated = (Port*) item;
                Port* portExisting;
                bool startAtBeginning;
                if(m_newLink->getPortFrom() == 0)
                {
                    startAtBeginning = false;
                    portExisting = m_newLink->getPortTo();
                }
                else //if(m_newLink->getPortTo() == 0)
                {
                    startAtBeginning = true;
                    portExisting = m_newLink->getPortFrom();
                }

                if (portCreated->getNode() != portExisting->getNode() && //Make sure the from and to nodes are not the same
                        portCreated->getPortType() != portExisting->getPortType() && //And both ports are not of the same type
                        !portCreated->isConnectedTo(portExisting)) //And the nodes are not already connected
                {
                    if(startAtBeginning)
                    {
                        if(!portExisting->getNode()->hasAncestor(portCreated->getNode()))
                        {
                            m_newLink->setPortTo(portCreated);
                        }
                        else
                        {
                            ///@todo Give a message that circularity is not appreciated
//                            std::cerr << "Sorry, no can do\n";
                            delete m_newLink;
                            m_newLink = 0;
                            return true;
                        }
                    }
                    else
                    {
                        if(!portCreated->getNode()->hasAncestor(portExisting->getNode()))
                        {
                            m_newLink->setPortFrom(portCreated);
                        }
                        else
                        {
                            ///@todo Give a message that circularity is not appreciated
//                            std::cerr << "Sorry, no can do\n";
                            delete m_newLink;
                            m_newLink = 0;
                            return true;
                        }
                    }
                    m_newLink->setPositionTo(portCreated->scenePos());
                    m_newLink->updatePath();
                    portCreated->addConnection(m_newLink);
                    portExisting->addConnection(m_newLink);

                    if(m_newLink->getPortFrom()->hasFileName())
                    {
                        m_newLink->getPortTo()->fileNameChanged(m_newLink->getPortFrom()->getFileName(), true);
                    }
                    else if(m_newLink->getPortTo()->hasFileName())
                    {
                        m_newLink->getPortFrom()->fileNameChanged(m_newLink->getPortTo()->getFileName(), true);
                    }
                    emit linkCreated(m_newLink->getPortFrom()->getNode(), m_newLink->getPortTo()->getNode());

                    m_newLink = 0;
                    return true;
                }
            }

            delete m_newLink;
            m_newLink = 0;
            return true;
        }
        break;
    }
    }
    return QObject::eventFilter(_object, _event);
}

void NodeEditor::keyPressEvent(
        QKeyEvent* _event
        )
{
    QGraphicsView::keyPressEvent(_event);
    switch (_event->key())
    {
    case Qt::Key_Backspace:
    {
        foreach (QGraphicsItem* item, scene()->items())
        {
            if(item->isSelected())
            {
                if(item->type() == Node::Type)
                {
                    //remove node from list view
                    m_treeModel->removeNode((const Node*) item);
                }
                delete item;
                //if no break, program may crash when second deleted item was already deletted by first on cascade
                break;
            }
        }
        break;
    }

    default:
    {
        break;
    }
    }
}

QGraphicsItem* NodeEditor::itemAt(
        const QPointF& _position
        )
{
    //Draw a square of 3 by 3 pixels around the input position and return the item
    QList<QGraphicsItem*> itemsWithinSquare = scene()->items(QRectF(_position - QPointF(1,1), QSize(3,3)));

    foreach(QGraphicsItem* eachItem, itemsWithinSquare)
    {
        if (eachItem->type() > QGraphicsItem::UserType)
        {
            return eachItem;
        }
    }
    return 0;
}

void NodeEditor::saveLinksToXml(
        QDomElement& _xmlElement
        )
{
    foreach(QGraphicsItem* item, scene()->items())
    {
        if (item->type() == Link::Type)
        {
            ((Link*) item)->saveToXml(_xmlElement);
        }
    }
}

void NodeEditor::loadFromXml(
        QDomDocument& _xmlFile
        )
{
//    std::cerr << "Loading file...\n";
    scene()->clear();

    QMap<quint64, Port*> portMap;

    QDomElement docElem = _xmlFile.documentElement();
    QDomNode n = docElem.firstChild();

    while(!n.isNull())
    {
        QDomElement e = n.toElement();
        if(e.tagName().compare("node") == 0)
        {
//            std::cerr << "Loading node...\n";
            Node* node = new Node(this);
            node->loadFromXml(e, portMap);
//            std::cerr << "Adding to tree...\n";
            getTreeModel()->addNode(node);
        }
        else if(e.tagName().compare("link") == 0)
        {
//            std::cerr << "Loading link...\n";
            Link* link = new Link(scene());
            link->loadFromXml(e, portMap);
        }
        n = n.nextSibling();
    }
}

void NodeEditor::setTreeModel(
        NodeTreeEditor* _treeModel
        )
{
    m_treeModel = _treeModel;
    connect(this, SIGNAL(linkCreated(const Node*, const Node*)), m_treeModel, SLOT(linkCreated(const Node*, const Node*)));
}


NodeTreeEditor* NodeEditor::getTreeModel(
        ) const
{
    return m_treeModel;
}

Node* NodeEditor::addNode(
        const NodeSetting* _setting
        )
{
    Node* node = new Node(this, _setting);
    m_treeModel->addNode(node);
    return node;
}

NodeEditor::~NodeEditor(
        )
{
    //The scene is created by MainWindow, but the pointer isn't saved. So it has to be destoyed here.
    delete scene();
    //For the weird instance that there is one and the editor is destroyed:
    delete m_newLink;
}
