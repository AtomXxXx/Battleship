#pragma once

enum GameState
{
    TitleScreen,
    ShipNotSelected, // while placing
    ShipSelected, // while placing
    PlayerTurn,
    AITurn,
    GameOver,
    Credits
};

enum GridState
{
    Empty,
    ShipPresent,
    ShipDestroyed,
    SelectedEmpty,
    Selected,
    Invalid
};
