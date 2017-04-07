#!/usr/bin/env python
import argparse
import fastr
import os


def write_tool(tool):
    xml = []
    xml.append("""<node>
  <category name="fastr">
    <category name="{namespace}"/>
  </category>
  <title name="{id} {version}">
    <code>
      <language name="fastr">
        <comment text="Tool **{id}** {version}"/>
        <argument name="('{ns_id}', '{version}')"/>
      </language>
    </code>
  </title>""".format(id=tool.id, ns_id=tool.ns_id, version=tool.version, namespace=tool.namespace))

    for input_ in tool.inputs.values():
        xml.append("""  <input editable="false" name="{id}">
    <code>
      <language name="fastr">
        <argument name="{id}"/>
      </language>
    </code>
  </input>""".format(id=input_.id))

    for output in tool.outputs.values():
        xml.append("""  <output editable="false" name="{id}">
    <code>
      <language name="fastr">
        <argument name="{id}"/>
      </language>
    </code>
  </output>""".format(id=output.id))

    xml.append('</node>')

    return '\n'.join(xml)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--out', type=str, required=True,
                        help='Output directory')
    args = parser.parse_args()

    base_path = args.out

    for tool in fastr.toollist.values():
        path = os.path.join(base_path, '{}_{}.node'.format(tool.id, tool.version))
        print('Converting {} to {}'.format(tool.ns_id, path))
        with open(path, 'w') as output:
            output.write(write_tool(tool))


if __name__ == '__main__':
    main()
