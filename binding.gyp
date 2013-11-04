{
  'targets': [
    {
      'target_name': 'nodesdr',
      'sources': [ 'nodesdr.cc', 'list_devs.cc', 'audioprocessor.cc', 'buffer.cc', 'testfilter.cc', 'filter.cc', 'firfilter.cc',
      'siggenfilter.cc', 'decfilter.cc', 'mixerfilter.cc'],
      'libraries': [ '-lportaudio', '-lfftw3', '-lm' ]
    }
  ]
}
