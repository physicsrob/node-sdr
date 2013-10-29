{
  'targets': [
    {
      'target_name': 'nodesdr',
      'sources': [ 'nodesdr.cc', 'list_devs.cc', 'rx.cc', 'buffer.cc', 'testfilter.cc', 'filter.cc', 'firfilter.cc',
      'siggenfilter.cc'],
      'libraries': [ '-lportaudio' ]
    }
  ]
}
