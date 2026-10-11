#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(112, 286);
inline const PParam KNIGHT_MAT = S(568, 1073);
inline const PParam BISHOP_MAT = S(518, 1013);
inline const PParam ROOK_MAT   = S(394, 1082);
inline const PParam QUEEN_MAT  = S(857, 1867);

inline const PParam TEMPO_VAL  = S(72, 59);

inline const PParam BISHOP_XRAY_PAWNS   = S(-14, -4);
inline const PParam BISHOP_PAIR_VAL   = S(62, 242);
inline const PParam ROOK_OPEN_VAL     = S(114, -6);
inline const PParam ROOK_SEMIOPEN_VAL = S(48, 10);
inline const PParam MINOR_BEHIND_PAWN = S(16, 44);
inline const PParam RESTRICTED_SQUARES = S(20, 5);
inline const PParam SPACE_VAL = S(95, -106);

inline const PParam DOUBLED_PAWN_VAL = S(-16, -89);
inline const PParam ISOLATED_PAWN_VAL = S(-12, -43);

inline const PParam POTENTIAL_CHECKER_VAL = S(-43, -50);
inline const PParam OUTPOST_KNIGHT_VAL    = S(50, 44);
inline const PParam OUTPOST_BISHOP_VAL    = S(43, 43);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(38, 53);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(42, 1);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(19, 66);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(63, -54);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(19, -13), S(40, 25), S(51, 79), S(115, 223), S(389, 460), S(809, 1022),
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(54, 24), S(42, 23), S(41, 72), S(86, 186), S(381, 143),
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-72, -183), S(-73, -142), S(-39, -18), S(9, 100), S(94, 327), S(233, 541),
};
inline const std::array<PParam, 6> PASSED_CLEAR_STOPPERS = {
    S(-62, 54), S(-67, 26), S(-43, 90), S(-78, 209), S(-33, 287), S(83, 184),
};
inline const std::array<PParam, 6> PASSED_CLEAR_FORWARD = {
    S(4, 58), S(-4, 47), S(-57, 102), S(-8, 106), S(107, 96), S(41, 185),
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(24, -34), S(18, 6), S(11, 41), S(24, 96), S(88, 232), S(400, 142),
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(5, -26), S(-3, 15), S(-9, -24), S(-6, -68), S(-22, -161), S(-188, -446),
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(16, 183), S(-1, 137), S(-7, 61), S(3, 21), S(17, 21), S(54, 12), S(61, -15),
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-134, -40), S(-4, -12), S(-11, 78), S(23, 114), S(36, 149), S(44, 161), S(31, 136),
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(52, -33), S(130, 236), S(160, 360), S(180, 415), S(204, 453), S(218, 494), S(237, 500), S(257, 506), S(276, 458),
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(92, 26), S(154, 260), S(207, 351), S(226, 424), S(242, 476), S(252, 514), S(257, 541), S(258, 565), S(261, 576), S(266, 585), S(279, 573), S(281, 566), S(282, 577), S(244, 535),
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(325, 399), S(238, 614), S(261, 646), S(276, 668), S(287, 689), S(290, 703), S(294, 722), S(298, 731), S(303, 743), S(307, 754), S(310, 764), S(308, 775), S(310, 781), S(315, 768), S(329, 734),
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(437, 499), S(560, 761), S(586, 912), S(604, 1063), S(624, 1105), S(632, 1156), S(636, 1206), S(638, 1234), S(641, 1264), S(643, 1281), S(647, 1296), S(653, 1300), S(655, 1308), S(661, 1307), S(661, 1313), S(664, 1310), S(659, 1316), S(661, 1303), S(660, 1302), S(670, 1288), S(652, 1288), S(673, 1248), S(634, 1284), S(588, 1283), S(552, 1275), S(513, 1298), S(442, 1359), S(482, 1269),
};

inline const PParam PAWN_THREAT_KNIGHT = S(205, 167);
inline const PParam PAWN_THREAT_BISHOP = S(167, 248);
inline const PParam PAWN_THREAT_ROOK   = S(196, 148);
inline const PParam PAWN_THREAT_QUEEN  = S(160, 25);

inline const std::array<std::array<PParam, 5>, 2> MINOR_THREAT = {{
  {{ S(9, 57), S(98, 101), S(102, 130), S(182, 92), S(170, 25), }},
  {{ S(6, 60), S(89, 91), S(101, 123), S(207, 629), S(163, -70), }},
}};
inline const std::array<std::array<PParam, 5>, 2> ROOK_THREAT = {{
  {{ S(9, 68), S(46, 117), S(66, 109), S(17, 19), S(162, -74), }},
  {{ S(-1, 57), S(65, 81), S(103, 68), S(16, 10), S(205, 1213), }},
}};
inline const PParam KING_THREAT  = S(20, 126);
inline const PParam HANGING_PAWN  = S(32, 91);
inline const PParam HANGING_NON_PAWN  = S(72, 29);

inline const std::array<PParam, 2> KNIGHT_ON_QUEEN = {
    S(19, -4), S(39, -10),
};
inline const std::array<PParam, 2> BISHOP_ON_QUEEN = {
    S(49, 37), S(116, -117),
};
inline const std::array<PParam, 2> ROOK_ON_QUEEN = {
    S(42, 5), S(48, -3),
};

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(6, -21), S(5, -18), S(1, -24), S(-5, -31), S(-12, -38), S(-17, -43), S(-19, -54), S(-25, -50), S(-25, -98),
};

inline const PParam ROOK_LINEUP = S(19, 80);

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(295, 464),    S(174, 573),    S(174, 446),    S(254, 499),    S(254, 433),    S(223, 468),    S(224, 479),    S(315, 422),    //
    S(127, 294),    S(126, 342),    S(102, 312),    S(107, 249),    S(100, 209),    S(77, 237),     S(84, 276),     S(63, 295),     //
    S(118, 243),    S(85, 273),     S(96, 264),     S(77, 197),     S(64, 182),     S(55, 205),     S(35, 213),     S(39, 247),     //
    S(75, 178),     S(42, 219),     S(66, 209),     S(59, 153),     S(38, 142),     S(33, 180),     S(-6, 191),     S(-3, 194),     //
    S(60, 140),     S(91, 143),     S(73, 223),     S(67, 181),     S(46, 172),     S(21, 184),     S(-10, 181),    S(-16, 178),    //
    S(84, 163),     S(161, 137),    S(159, 163),    S(120, 198),    S(93, 177),     S(64, 179),     S(23, 191),     S(9, 192),      //
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-167, -318),  S(-165, 273),   S(-183, 333),   S(120, 276),    S(25, 278),     S(-202, 342),   S(-239, 314),   S(-259, -230),  //
    S(100, 217),    S(88, 250),     S(183, 236),    S(182, 311),    S(201, 280),    S(152, 248),    S(89, 234),     S(45, 241),     //
    S(131, 197),    S(132, 263),    S(137, 296),    S(164, 321),    S(150, 319),    S(110, 291),    S(93, 263),     S(52, 228),     //
    S(181, 256),    S(178, 292),    S(184, 316),    S(193, 359),    S(196, 343),    S(164, 302),    S(144, 252),    S(119, 257),    //
    S(170, 238),    S(192, 226),    S(196, 275),    S(182, 320),    S(180, 303),    S(161, 287),    S(152, 233),    S(119, 223),    //
    S(114, 158),    S(140, 177),    S(143, 235),    S(153, 278),    S(150, 271),    S(124, 225),    S(109, 190),    S(70, 155),     //
    S(133, 172),    S(125, 207),    S(121, 177),    S(131, 215),    S(127, 213),    S(99, 161),     S(56, 210),     S(41, 112),     //
    S(44, 104),     S(108, 158),    S(120, 161),    S(134, 179),    S(120, 182),    S(79, 153),     S(67, 180),     S(7, 65),       //
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(71, 325),     S(-45, 470),    S(-222, 544),   S(-97, 470),    S(-113, 500),   S(-83, 495),    S(-15, 459),    S(34, 439),     //
    S(144, 330),    S(102, 451),    S(120, 429),    S(117, 429),    S(121, 434),    S(144, 392),    S(158, 390),    S(122, 365),    //
    S(218, 368),    S(193, 402),    S(205, 420),    S(201, 413),    S(201, 397),    S(197, 401),    S(174, 398),    S(200, 332),    //
    S(185, 347),    S(223, 385),    S(241, 389),    S(259, 423),    S(272, 413),    S(219, 377),    S(214, 359),    S(176, 340),    //
    S(210, 319),    S(213, 360),    S(249, 369),    S(267, 389),    S(256, 388),    S(241, 376),    S(194, 358),    S(180, 317),    //
    S(229, 328),    S(253, 325),    S(268, 351),    S(240, 388),    S(236, 367),    S(232, 365),    S(235, 345),    S(198, 319),    //
    S(202, 278),    S(295, 267),    S(236, 325),    S(222, 339),    S(210, 335),    S(221, 298),    S(227, 279),    S(209, 309),    //
    S(209, 282),    S(197, 321),    S(204, 336),    S(211, 322),    S(198, 320),    S(211, 360),    S(214, 333),    S(208, 291),    //
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(350, 618),    S(372, 643),    S(331, 686),    S(318, 647),    S(371, 617),    S(324, 638),    S(349, 644),    S(342, 630),    //
    S(288, 640),    S(326, 658),    S(379, 619),    S(332, 613),    S(374, 603),    S(340, 620),    S(287, 654),    S(283, 648),    //
    S(266, 617),    S(372, 573),    S(409, 559),    S(369, 547),    S(375, 568),    S(314, 602),    S(297, 625),    S(255, 658),    //
    S(232, 589),    S(292, 604),    S(297, 600),    S(282, 592),    S(312, 568),    S(283, 598),    S(268, 614),    S(222, 631),    //
    S(207, 521),    S(258, 545),    S(243, 581),    S(209, 587),    S(227, 579),    S(219, 600),    S(206, 595),    S(181, 596),    //
    S(217, 428),    S(288, 457),    S(267, 493),    S(241, 512),    S(252, 515),    S(237, 532),    S(238, 504),    S(192, 519),    //
    S(145, 416),    S(267, 379),    S(263, 432),    S(247, 477),    S(255, 472),    S(240, 485),    S(234, 464),    S(205, 456),    //
    S(207, 381),    S(227, 418),    S(286, 420),    S(294, 418),    S(299, 425),    S(275, 449),    S(273, 436),    S(253, 435),    //
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(499, 820),    S(574, 793),    S(457, 960),    S(409, 1023),   S(432, 1016),   S(486, 912),    S(477, 843),    S(428, 868),    //
    S(465, 836),    S(472, 893),    S(431, 977),    S(323, 1074),   S(372, 1050),   S(435, 907),    S(471, 818),    S(455, 779),    //
    S(434, 853),    S(468, 893),    S(430, 971),    S(404, 990),    S(421, 998),    S(464, 890),    S(500, 776),    S(483, 746),    //
    S(430, 798),    S(436, 863),    S(396, 930),    S(401, 976),    S(427, 963),    S(431, 864),    S(477, 775),    S(459, 738),    //
    S(418, 776),    S(420, 777),    S(416, 827),    S(395, 892),    S(404, 919),    S(431, 854),    S(435, 792),    S(452, 712),    //
    S(419, 667),    S(454, 649),    S(445, 739),    S(420, 770),    S(436, 774),    S(435, 814),    S(458, 743),    S(440, 725),    //
    S(392, 487),    S(432, 461),    S(432, 555),    S(455, 616),    S(454, 654),    S(457, 635),    S(436, 662),    S(447, 647),    //
    S(367, 504),    S(414, 310),    S(435, 351),    S(442, 466),    S(459, 535),    S(457, 532),    S(453, 544),    S(420, 610),    //
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(339, -623),   S(550, 62),     S(410, 286),    S(5, 424),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(411, -253),   S(358, 166),    S(194, 325),    S(-46, 373),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(214, -20),    S(221, 136),    S(47, 311),     S(-178, 413),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(40, -43),     S(116, 66),     S(-92, 291),    S(-248, 426),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-87, -79),    S(-16, 18),     S(-101, 210),   S(-252, 350),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-40, -111),   S(20, -36),     S(-75, 161),    S(-159, 278),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(44, -182),    S(46, -99),     S(-28, 54),     S(-119, 178),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-8, -283),    S(19, -168),    S(-70, -22),    S(-80, 18),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
};

inline const PParam KS_NO_QUEEN = S(-72, -75);

inline const std::array<PParam, 5> PT_INNER_RING_ATTACKS = {
    S(7, -1), S(11, 1), S(10, 1), S(5, -1), S(3, -5),
};
inline const std::array<PParam, 5> PT_OUTER_RING_ATTACKS = {
    S(3, -2), S(6, -1), S(3, -1), S(3, -0), S(3, 0),
};

inline const PParam KS_FLANK_ATTACK = S(4, -3);
inline const PParam KS_FLANK_DEFENSE = S(-2, -1);
inline const PParam KS_FLANK_DOUBLE_ATTACK   = S(3, -1);
inline const PParam KS_FLANK_DOUBLE_DEFENSE  = S(-2, 1);

inline const std::array<std::array<PParam, 7>, 4> KING_SHELTER = {{
  {{ S(13, -2), S(-7, 11), S(-3, -1), S(6, -4), S(13, -12), S(3, -17), S(4, -38), }},
  {{ S(2, 13), S(-18, 6), S(-13, 10), S(-4, 11), S(-1, 8), S(-7, 0), S(-11, -18), }},
  {{ S(-3, 21), S(-10, 12), S(-8, 24), S(-1, 22), S(-1, 20), S(-6, 15), S(1, -366), }},
  {{ S(1, 27), S(-8, 17), S(-1, 21), S(4, 19), S(4, 16), S(4, 22), S(6, 15), }},
}};
inline const std::array<PParam, 7> BLOCKED_SHELTER_STORM = {
    S(0, 0), S(0, 0), S(-6, 24), S(-19, 7), S(-20, 12), S(-17, 17), S(-3, 15),
};
inline const std::array<std::array<PParam, 7>, 4> SHELTER_STORM = {{
  {{ S(-7, 11), S(-49, -309), S(-14, -54), S(-8, -4), S(-13, 2), S(-16, 7), S(-16, 7), }},
  {{ S(-3, 3), S(-29, -279), S(-8, -58), S(-9, -12), S(-11, -4), S(-19, 4), S(-10, 3), }},
  {{ S(-10, 7), S(-4, -247), S(5, -33), S(-5, -7), S(-12, 3), S(-18, 8), S(-15, 9), }},
  {{ S(-9, 1), S(4, -74), S(5, -25), S(-6, 3), S(-16, 6), S(-21, 3), S(-18, 5), }},
}};
inline TunableSigmoid<32> KING_SAFETY_ACTIVATION(
        1506, 1391, -19, -5
);

inline VParam WINNABLE_PAWNS = V(-21);
inline VParam WINNABLE_SYM = V(107);
inline VParam WINNABLE_ASYM = V(94);
inline VParam WINNABLE_PAWN_ENDGAME = V(73);
inline VParam WINNABLE_BIAS = V(-412);

// Epoch duration: 4.22718s
// clang-format on
}  // namespace Clockwork
