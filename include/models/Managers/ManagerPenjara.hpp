#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include "../Petak/Petak.hpp"
#include "../Petak/PetakAksi.hpp"
#include "../Pemain.hpp"
#include "../ConfigData.hpp"
#include "../Dadu.hpp"
#include "../PlayerActionService.hpp"

class ManagerPenjara {
protected :
public :
    void tryEscapeByDouble (Pemain &p, Dadu &d, PlayerActionService actionService);

    void escapeByFine (Pemain &p, PlayerActionService actionService);

    void paksaBayar (Pemain &p, PlayerActionService actionService); // udah turn4
};