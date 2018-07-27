Dirty patch for German umlaut. We should use a standard UTF16 upper case function for this.
- "towupper()" appears to not work for this use-case. Tried setting locale and also tried other solution. Used http://www.leighb.com/tounicupper.htm for mapping.
