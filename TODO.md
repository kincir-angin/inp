sudo in create-package ./test

Creating tarball...

meta-data.json:

```json
{
  "project": {
    "name": "Linux API Kernel",
    "version": "6.0-rc1",
    "description": "Lorem ipsum dolor sit amet",
    "author": "Naufal Adriansyah",
    "organization": "Kincir Angin Studio",
    "contents": "content.json"
  },
  "license": {
    "name": "GNU General Public License",
    "version": 3.0,
    "content": "LICENSE"
  }
}
```

LICENSE:

```
Copyright (C) <year>  <name of author>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
```

If missing: LICENSE
  missing LICENSE file.

if missing: meta-info.json 
  missing meta-info.json, do you wan't to create it? (Y/n)

  if lower(answer) == 'y':
    > Project Name: test
    > Author: Test 
    > 
  else:
    exiting...