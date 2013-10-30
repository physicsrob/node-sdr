{
  'targets': [
    {
      'target_name': 'nodesdr',
      'sources': [ 'nodesdr.cc', 'list_devs.cc', 'rxprocessor.cc', 'buffer.cc', 'testfilter.cc', 'filter.cc', 'firfilter.cc',
      'siggenfilter.cc', 'decfilter.cc'],
      'libraries': [ '-lportaudio' ]
    }
  ]
}
