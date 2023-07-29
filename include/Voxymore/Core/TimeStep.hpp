//
// Created by ianpo on 25/07/2023.
//

#pragma once

namespace Voxymore {
    namespace Core {

        struct TimeStep {
        public:
            TimeStep(float timeInSeconds = 1.0f);

            inline operator float() const { return m_Time; }

            inline float GetSeconds() const {return m_Time;}
            inline float GetMilliSeconds() const {return m_Time * 1000.0f;}
        private:
            float m_Time;
        };

    } // Voxymore
} // Core
