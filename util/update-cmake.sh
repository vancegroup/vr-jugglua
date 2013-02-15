#!/bin/sh -e
(
  cd "$(git rev-parse --show-toplevel)"
  git subtree pull --squash --prefix=cmake https://github.com/rpavlik/cmake-modules.git master
)
