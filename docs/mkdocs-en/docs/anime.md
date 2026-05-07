Anime
=====

Anime is a feature to play layer-based animations via the anime tag.

## Anime File

An anime file is a text file which describes sequences of layer transforms.

## Sequence

To move the message box 100px right in a second, write the following sequence in an anime file.

```
# A block describes a sequence for animation.
# The name of a block can be whatever you like and it won't affect anything.
move {
    # This is a layer specifier.
    layer: msg;

    # These are time specifiers. (in second)
    start: 0.0;
    end: 1.0;

    # These are origin position specifiers. 'r0' means relative '0'.
    from-x: r0;
    from-y: r0;

    # This is an origin alpha specifier.
    from-a: 255;

    # These are final position specifiers. 'r100' means relative '100'.
    to-x: r100;
    to-y: r0;

    # This is a final alpha specifier.
    to-a: 255;
}
```

## Layer Structure

The following is our layer structure in the bottom-to-top order.

| Layer Name       | Description                                |
|------------------|--------------------------------------------|
| bg               | background                                 |
| bg2              | second background (for seemless scrolling) |
| efb1 ~ efb4     | effect on back 1 ~ 4  |
| chb              | character at back center                   |
| chb-eye          | character at back center                   |
| chb-lip          | character at back center                   |
| chl              | character at left                          |
| chl-eye          | character at left                          |
| chl-lip          | character at left                          |
| chlc             | character at left center                   |
| chlc-eye         | character at left center                   |
| chlc-lip         | character at left center                   |
| chr              | character at right                         |
| chr-eye          | character at right                         |
| chr-lip          | character at right                         |
| chrc             | character at right center                  |
| chrc-eye         | character at right center                  |
| chrc-lip         | character at right center                  |
| eff1 ~ eff4      | effect on front 1 ~ 4  |
| msgbox           | message box                                |
| namebox          | name box                                   |
| choose1-idle ~ choose8-idle  | choose box 1 ~ 8 (idle)      |
| choose1-hover ~choose8-hover  | choose box 1 ~ 8 (hover)   |
| chf              | character face                             |
| chf-eye          | character face                             |
| chf-lip          | character face                             |
| click            | click animation                            |
| auto             | auto mode banner                           |
| skip             | skip mode banner                           |
| text1 ~ text8    | text layer 1 ~ 8               |
| gui-button-1 ~ gui-button-32   | GUI button ID 1 ~ 32    |

## Scaling and Rotation

```
# Scale-up and rotate the `effect1` layer to 2.0x and 360 deg in 3 seconds.
test1 {
    layer: effect1;
 
    start: 0.0;
    end: 3.0;

    from-x: 0;
    from-y: 400;
    from-a: 255;

    to-x: 0;
    to-y: 400;
    to-a: 255;

    # Scaling and rotation origin
    center-x: 600;
    center-y: 100;

    # Scaling factors
    from-scale-x: 1.0;
    from-scale-y: 1.0;
    to-scale-x: 2.0;
    to-scale-y: 2.0;
 
    # Rotation (+ for right, - for left)
    from-rotate: 0.0;
    to-rotate: -360;
}

# Reverse.
test2 {
    layer: effect1;

    start: 3.0;
    end: 6.0;

    from-x: 0;
    from-y: 400;
    from-a: 255;

    to-x: 0;
    to-y: 400;
    to-a: 255;

    center-x: 600;
    center-y: 100;

    from-scale-x: 2.0;
    to-scale-x: 1.0;

    from-scale-y: 2.0;
    to-scale-y: 1.0;

    from-rotate: -360;
    to-rotate: 0;
}
```
