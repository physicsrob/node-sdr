{
  'targets': [
    {
      'target_name': 'nodesdr',
      'sources': [ 'audio/nodesdr.cc', 'audio/list_devs.cc', 'audio/audioprocessor.cc', 'audio/buffer.cc', 'audio/testfilter.cc', 'audio/filter.cc', 
      'audio/firfilter.cc', 'audio/siggenfilter.cc', 'audio/decfilter.cc', 'audio/mixerfilter.cc'],
      'libraries': [ '-lportaudio', '-lfftw3', '-lm' ]
    }
  ]
}
