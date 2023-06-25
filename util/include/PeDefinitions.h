// Copyright 2020-2023 Paul Robertson
//
// PeDefinitions.h
//
// General code, type etc. definitions

#pragma once

namespace pe
{
// Typedefs for integral types
// Using long long types by default since lots of problems in Project Euler
// seem to involve numbers that can hit 32 bit overflow easily (especially
// when generalised). They can hit 64 bit overflow too, but this gives us
// more leeway.
// This naturally will affect performance, but part of the purpose of this
// project is to explore and illustrate math, algorithms and coding, rather
// than to try to make the fastest possible implementations.
typedef long long unsigned PeUint;
typedef long long signed   PeInt;
}; // namespace pe
