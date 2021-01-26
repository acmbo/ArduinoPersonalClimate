Imports System.IO
Imports System.IO.Ports

Public Class Form1

    Dim value1 As Decimal
    Dim value2 As Decimal


    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load

        'SerialPort1.Close()
        'SerialPort1.PortName = "com5"
        'SerialPort1.BaudRate = "9600"
        'SerialPort1.DataBits = 8
        'SerialPort1.Parity = Parity.None
        'SerialPort1.StopBits = StopBits.One
        'SerialPort1.Handshake = Handshake.None
        'SerialPort1.Encoding = System.Text.Encoding.Default
        'SerialPort1.Open()
    End Sub



    Private Sub Timer1_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Timer1.Tick
        Dim s As String

        s = TextBox1.Text + "," + "," + "," + ","

        Dim somestring() As String
        ' Split string based on comma
        somestring = s.Split(New Char() {","c})

        TextBox2.Text = somestring(0)
        Try
            value1 = Convert.ToDecimal(TextBox2.Text)

        Catch ex As Exception
            TextBox1.Text = ""
        End Try

        TextBox3.Text = somestring(1)
        Try
            value2 = Convert.ToDecimal(TextBox3.Text)

        Catch ex As Exception
            TextBox1.Text = ""
        End Try
        TextBox4.Text = somestring(2)


        Chart1.Series("Humidity").Points.AddY(value2)
        Chart1.Series("Temperature").Points.AddY(value1)
        TextBox1.Text = ""



    End Sub

    Private Sub DataReceived(ByVal sender As Object, ByVal e As SerialDataReceivedEventArgs) Handles SerialPort1.DataReceived


        Try

            Dim mydata As String = ""
            mydata = SerialPort1.ReadExisting()

            If TextBox1.InvokeRequired Then
                TextBox1.Invoke(DirectCast(Sub() TextBox1.Text &= mydata, MethodInvoker))
            Else
                TextBox1.Text &= mydata
            End If
        Catch ex As Exception
            MessageBox.Show(ex.Message)
        End Try
    End Sub


End Class
