# FriendGD

Universal Friendship is a Geode mod for Geometry Dash that pretends every user is on your friend list. When you open a profile you will always see the messaging and friendship controls as if you were already connected.

## Features

- Hooks the internal friendship checks so any profile is treated as a friend.
- Forces the message and block buttons to render as they would for existing friends.
- Keeps the original backend messaging flow while skipping the "friends only" requirement.
- Includes a Geode setting toggle to enable/disable the simulation and an optional partial mode that only spoofs non-friends.
- Logs recoverable errors encountered while patching the profile UI, helping diagnose conflicts with other mods.

## Building

This project follows the standard [Geode](https://github.com/geode-sdk/geode) CMake layout. Install the SDK with the Geode CLI and make sure the `GEODE_SDK_PATH` environment variable points to your Geode SDK folder (the CLI command `geode setup sdk` will do both).

```bash
cmake -B build
cmake --build build
```

Then place the generated `.geode` package in your Geometry Dash mods folder.
