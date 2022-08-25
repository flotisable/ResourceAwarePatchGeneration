# Resource Aware Patch Generation

  a project trys to solve [the problem A of CAD contest 2017](http://cad-contest-2017.el.cycu.edu.tw/Problem_A/default.html)

## Status

  - [x] alogrithm for single target function
  - [ ] alogrithm for multiple target function
  - [ ] code

## Dependency

  - **abc** : [abc berkeley](https://people.eecs.berkeley.edu/~alanmi/abc/)
    - the code of **abc** is moved to [github](https://github.com/berkeley-abc/abc)
      but seems not mentioned in the website
    - the **abc** is added as submodule in the repo to make sure the repo is
      self conatined and will not compile fail when the API of **abc** changes.
      To compile with other version of **abc**, use one of the following method
      - go to directory ```Lib/abc``` and ```git pull``` to update the **abc**
        repo, then ```git checkout``` to the version you want
      - if you have abc version in other directory, modify the **abcDir**
        variable in makefile to the root of your **abc** directory

## Compile

  - check 'getAbcFlags.sh' is executable
  - run `make`
