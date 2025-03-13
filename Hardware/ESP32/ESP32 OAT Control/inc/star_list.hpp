#pragma once

// 일단 별을 여기에 저장하고 나중에 될 수 있으면 파일에 저장할 수 있도록 변경하기

#include <Arduino.h>
#include <vector>

class Star {
private:
    String catalog;
    String name;
    String ra;
    String dec;

public:
    Star(const String catalog, const String name, const String ra, const String dec);

    class Get {
	private:
		Star& star;
    public:
        Get(Star& star) : star(star) {}

		String& catalog() {
			return star.catalog;
		}
        String& name() {
            return star.name;
        }
		String& ra() {
			return star.ra;
		}
        String& dec() {
            return star.dec;
        }
    };

	class Set {
	private:
		Star& star;
	public:
		Set(Star& star) : star(star) {}

		void catalog(const String catalog) {
			star.catalog = catalog;
		}
        void name(const String name) {
            star.name = name;
        }
		void ra(const String ra) {
			star.ra = ra;
		}
        void dec(const String dec) {
            star.dec = dec;
        }
	};

    Get get() {
        return Get(*this);
    }

	Set set() {
		return Set(*this);
	}
};

Star::Star(const String catalog, const String name, const String ra, const String dec) {
    this->catalog = catalog;
    this->name = name;
    this->ra = ra;
    this->dec = dec;
}

class StarList {
private:
    std::vector<Star> stars;

public:
    StarList() {
        stars = {
            Star(""    , "Polaris (Celestial Pole)" , "02:58:51", "89:21:13" ),
            Star("M81" , "Bodes Galaxy"             , "09:57:13", "68:58:01" ),
            Star("M82" , "Cigar Galaxy"             , "09:57:52", "69:33:55" ),
            Star(""    , "Cederblad 214"            , "00:05:46", "67:16:45" ),
            Star(""    , "Heart Nebula - Cassiopeia", "02:34:57", "61:31:17" ),
            Star(""    , "Soul Nebula - Cassiopeia" , "02:52:47", "60:30:56" ),
            Star(""    , "Navi - Cassiopeia"        , "00:57:57", "60:49:33" ),
            Star(""    , "Elephant Trunk Nebula"    , "21:39:44", "57:35:31" ),
            Star(""    , "Big Dipper"               , "12:16:26", "56:55:07" ),
            Star("M74 ", "Phantom Galaxy"           , "01:36:42", "15:47:01" ),
            Star("M100", "Mirror Galaxy"            , "12:22:55", "15:49:21" ),
            Star("M101", "Pinwheel Galaxy"          , "14:03:56", "54:15:00" ),
            Star("M51" , "Whirlpool Galaxy"         , "13:30:45", "47:05:21" ),
            Star(""    , "Deneb - Cygnus Nebula"    , "20:42:07", "45:21:12" ),
            Star("M63" , "Sunflower Galaxy"         , "13:15:49", "42:01:14" ),
            Star("M31" , "Andromeda Galaxy"         , "00:43:52", "41:22:53" ),
            Star(""    , "Vega"                     , "18:37:37", "38:48:07" ),
            Star(""    , "Veil Nebula"              , "20:51:19", "31:03:34" ),
            Star("M33" , "Triangulum Nebula"        , "01:34:59", "30:45:40" ),
            Star("M45" , "Pleiades 7 Sisters"       , "03:48:15", "24:10:54" ),
            Star(""    , "Arcturus"                 , "14:16:37", "19:05:21" ),
            Star(""    , "Altair"                   , "19:51:45", "08:55:15" ),
            Star("M42" , "Orion Nebula"             , "05:36:18", "-5:22:44" ),
            Star("M8"  , "Lagoon Nebula"            , "18:05:02", "-24:22:52"),
            Star(""    , "Cocoon Nebula"            , "21:55:13", "47:22:07" ),
            Star(""    , "Crescent Nebula"          , "20:12:54", "38:25:11" ),
            Star(""    , "Horsehead Nebula"         , "05:42:03", "-2:26:52" ),
            Star(""    , "Bubble Nebula"            , "23:21:45", "61:19:10" ),
            Star(""    , "California Nebula"        , "04:04:42", "36:28:50" ),
            Star(""    , "Christmas Tree Cluster"   , "06:42:09", "09:52:07" ),
            Star(""    , "Rosette Nebula"           , "06:33:03", "04:55:33" ),
            Star("M20" , "Trifid Nebula"            , "18:02:23", "-23:01:48"),
            Star("M16" , "Eagle Nebula"             , "18:19:01", "-13:49:00"),
            Star("M27" , "Dumbbell Nebula"          , "20:00:42", "22:47:11" ),
            Star("M94" , "Croc's Eye Galaxy"        , "12:50:53", "41:07:12" ),
            Star("M57" , "Ring Nebula"              , "18:53:35", "33:01:45" ),
            Star("M104", "Sombrero Galaxy"          , "12:41:06", "-11:44:28"),
            Star("M109", "Vacuum Cleaner Galaxy"    , "11:57:36", "52:22:28" ),
            Star(""    , "Seagull Nebula"           , "07:05:26", "-10:29:16"),
            Star(""    , "Dolphin Nebula"           , "01:31:57", "58:31:29" ),
            Star(""    , "Cave Nebula"              , "22:58:08", "62:35:27" ),
            Star(""    , "Wizard Nebula"            , "22:48:12", "58:14:44" )
        };
    }

    Star* findByName(const String& name) {
        for(auto& star : stars) {
            if(star.get().name() == name) {
                return &star;
            }
        }
        return nullptr;
    }

    std::vector<Star*> findByCatalog(const String& catalog) {
        std::vector<Star*> found;
        for(auto& star : stars) {
            if(star.get().catalog() == catalog) {
                found.push_back(&star);
            }
        }
        return found;
    }

    std::vector<Star>& getAllStars() {
        return stars;
    }
};