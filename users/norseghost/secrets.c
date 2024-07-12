#include "norseghost.h"
#if (__has_include("secrets.h") && !defined(NO_SECRETS))
#    include "secrets.h"
#endif

bool process_record_secrets(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SECRET_1 ... SECRET_5: // Secrets!  Externally defined strings, not stored in repo
            if (record->event.pressed) {
                clear_mods();
                clear_oneshot_mods();
                send_string_with_delay(secrets[keycode - SECRET_1], MACRO_TIMER);
            }
            return false;
            break;
    }
    return true;
}
