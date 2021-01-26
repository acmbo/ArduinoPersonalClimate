Imports System.Text
Imports System
Imports System.IO

Public Class Form2
    Private Sub Form2_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        Dim ThisMoment As Date
        ThisMoment = Now
        Me.RichTextBox1.Text = Form1.RichTextBox1.Text
        TextBox1.Text = "C:/sers/Stephan/Documents/Arduino/Data"
        TextBox2.Text = "Dataset" & ThisMoment & ".txt"
    End Sub


    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        Dim file1 As System.IO.StreamWriter
        Dim Dataset As Byte() = New UTF8Encoding(True).GetBytes(RichTextBox1.Text)
        Dim path As String
        path = TextBox1.Text & TextBox2.Text
        Dim fs As FileStream = File.Create(path)


        'Dataset = RichTextBox1.Text
        'file1.Write(Dataset)
        fs.Write(Dataset, 0, Dataset.Length)
        fs.Close()
        'file1 = My.Computer.FileSystem.OpenTextFileWriter(TextBox1.Text & TextBox2.Text, True)


    End Sub


End Class