/**************************************************
 *  Asymp
 *     Taken from the CCRMA Synthesis Toolkit:
 *
 *     \class Asymp
 *     \brief STK asymptotic curve envelope class
 *
 *     This class implements a simple envelope generator
 *     which asymptotically approaches a target value.
 *     The algorithm used is of the form:
 *
 *     y[n] = a y[n-1] + (1-a) target,
 *
 *     where a = exp(-T/tau), T is the sample period, and
 *     tau is a time constant.  The user can set the time
 *     constant (default value = 0.3) and target value.
 *     Theoretically, this recursion never reaches its
 *     target, though the calculations in this class are
 *     stopped when the current value gets within a small
 *     threshold value of the target (at which time the
 *     current value is set to the target).  It responds
 *     to \e keyOn and \e keyOff messages by ramping to
 *     1.0 on keyOn and to 0.0 on keyOff.
 *
 *     by Perry R. Cook and Gary P. Scavone, 1995--2017.
 */

#pragma once
namespace meta
{
    class Asymp
    {
    public:

        //! Default constructor.
        Asymp() {};

        /// If the sample rate is already known
        explicit Asymp(double sampleRate);

        //! Class destructor.
        ~Asymp();

        // Reset the entire object to a ready state
        void init(double sampleRate);

        //! Set the asymptotic rate via the time factor \e tau (must be > 0).
        /*!
          The rate is computed as described above.  The value of \e tau
          must be greater than zero.  Values of \e tau close to zero produce
          fast approach rates, while values greater than 1.0 produce rather
          slow rates.
        */
        void setTau(float tau, float sampleRate);

        //! Set the asymptotic rate based on a time duration (must be > 0).
        void setTime(float time, float sampleRate);

        //! Set the asymptotic rate such that the target value is perceptually reached (to within -60dB of the target) in \e t60 seconds.
        void setT60(float t60, float sampleRate);

        //! Set the target value.
        void setTarget(float target);

        //! Set current and target values to \e value.
        void setValue(float value);

        //! Return the current envelope \e state (0 = at target, 1 otherwise).
        int getState() const { return m_State; };

        //! Compute and return one output sample.
        float tick();

    protected:
        float m_Value;
        float m_Target;
        float m_Factor;
        float m_Constant;
        int m_State;

        static const float TARGET_THRESHOLD;
    };

    inline float Asymp::tick(void)
    {
        if (m_State)
        {

            m_Value = m_Factor * m_Value + m_Constant;

            // Check threshold.
            if (m_Target > m_Value)
            {
                if (m_Target - m_Value <= TARGET_THRESHOLD)
                {
                    m_Value = m_Target;
                    m_State = 0;
                }
            } else
            {
                if (m_Value - m_Target <= TARGET_THRESHOLD)
                {
                    m_Value = m_Target;
                    m_State = 0;
                }
            }
        }

        return m_Value;
        
        static const float TARGET_THRESHOLD = 0.000001;
    }
}
