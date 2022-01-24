# Musical Emergence

Music with particle physics!

## About

This is a midi plugin written in JUCE to audiate particle physics through different MIDI synths. Simply provide MIDI messages as input to create new particles, and the particles' collisions produce new MIDI messages of their own which are sent out to be synthesized. Pitch corresponds to the size and color of the particles, while collisions with larger change in momentum create louder sounds.

The collisions are calculated using the Sweep and Prune method, more information [here](http://codercorner.com/SAP.pdf). This offers significant performance improvement over the O(n^2) approach, albeit with a larger memory footprint for storing the collision matrix.

## Controls

- Input MIDI messages with a MIDI controller or virtual keyboard to create particles with a specific pitch
- Click and drag across the particle box to add neutral particles
- Select "Show AABB's" to display the Axis-Aligned Bounding Boxes used by the SAP algorithm along with the current collision pairs
- Change the simulation speed using the slider in the sidebar
- Pause the simulation with the button in the sidebar

## Installation

1. Clone the repository and open the .jucer file in the Projucer
2. Generate the build files for your system
3. Build the project using your IDE or the generated Makefile

## Future Improvements

- Add support for saving state
- Add emergence, similar to [this project](https://github.com/ncase/fireflies)
- Add sampling support
